#ifndef __FLICKROAUTH_H__
#define __FLICKROAUTH_H__

#include <flickr_request.h>

#include <QByteArray>

class QNetworkAccessManager;
class QNetworkReply;

namespace app {

    /*
     * Manages the addition of common OAuth request parameters
     * - oauth_consumer_key
     * - oauth_nonce
     * - oauth_signature_method
     * - oauth_signature
     * - oauth_timestamp
     */
    class FlickrOAuthRequest : public FlickrGetRequest {
        Q_OBJECT

        public: // methods
        FlickrOAuthRequest(const QString&, QObject* parent=0);
        virtual ~FlickrOAuthRequest();

        const QByteArray& getConsumerKey() const;
        void setConsumerKey(const QByteArray&);
        virtual QNetworkReply* send(QNetworkAccessManager&);

        private: // data members
        QByteArray consumerKey;
    };

    /*
     * Manages the Flickr OAuth authentication process
     */

    class FlickrOAuthAuthentication : public QObject {
        Q_OBJECT

        public: // types
        enum State {
            Initialized,
            GettingRequestToken,
            RequestTokenReceived,
            GettingUserAuthorization,
            UserAuthorizationReceived,
            GettingAccessToken,
            AccessTokenReceived,
            Error
        };

        public: // methods
        FlickrOAuthAuthentication(QObject* parent = 0);
        FlickrOAuthAuthentication(QNetworkAccessManager& networkAccessMan, QObject* parent = 0);
        virtual ~FlickrOAuthAuthentication();

        void authenticate();

        private: // methods
        void sendRequestTokenRequest();

        private: // slots
        Q_SLOT void requestTokenRequestResponseReceived();

        private: // data members
        State state; 

        QByteArray userName;
        QNetworkAccessManager* networkAccessMan;
        QNetworkReply* flickrResponse;
    };
}

#endif
