/**
 *  Qt Flickr Client
 *  Copyright (C) 2014 Aruth Kandage
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.*/ 

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

        private: // methods

        // QObject derived classes should not be copiable
        Q_DISABLE_COPY(FlickrOAuthRequest);

        private: // data members
        QByteArray consumerKey;
    };

    /*
     * Manages the Flickr OAuth authentication process
     */

    class FlickrOAuthManager : public QObject {
        Q_OBJECT

        public: // types
        enum State {
            /*
             * Flickr uses OAuth 1.0 with the flow as follows
             * 1. Ask for a 'request token'
             * 2. Redirect the user to give the application certain permissions
             * 3. Receive an 'oauth verifier'
             * 4. Exchange the 'request token' for the 'access token'
             */ 

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
        FlickrOAuthManager(QObject* parent = 0);
        FlickrOAuthManager(QNetworkAccessManager& networkAccessMan, QObject* parent = 0);
        virtual ~FlickrOAuthManager();

        void authenticate();
        State getState() const;

        public: // signals
        // Emitted when an error has occurred
        Q_SIGNAL void error();

        private: // methods
        void setState(State);

        // QObject derived classes should not be copiable
        Q_DISABLE_COPY(FlickrOAuthManager);

        void destroyResponse();
        void requestRequestToken();
        void extractRequestTokenAndSecret();

        private: // slots
        Q_SLOT void requestTokenResponseReceived();

        private: // data members
        State state; 

        QByteArray userName;
        QByteArray token; 
        QByteArray tokenSecret;
        QByteArray verifier;
        QNetworkAccessManager* networkAccessMan;
        QNetworkReply* flickrResponse;
    };
}

#endif
