#ifndef __FLICKROAUTH_H__
#define __FLICKROAUTH_H__

#include <flickr_request.h>

#include <QByteArray>

class QNetworkAccessManager;
class QNetworkReply;

namespace app {

    class FlickrOAuthRequest : public FlickrGetRequest {
        Q_OBJECT
        
        public: // methods

        private: // data members
    };

    /*
     * Manages the Flickr OAuth authentication process
     */

    class FlickrOAuthAuthentication : public QObject {
        Q_OBJECT

        public: // types
        enum OAuthFlowState {
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
        FlickrOAuthAuthentication(QNetworkAccess* networkAccessMan, QObject* parent = 0);

        void authenticate();

        private: // data members
        QByteArray userName;
        QNetworkAccessManager* networkAccessMan;
        QNetworkReply* flickrResponse;
    };
}

#endif
