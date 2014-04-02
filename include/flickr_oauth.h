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
