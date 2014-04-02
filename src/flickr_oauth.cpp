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

#include <flickr_api_key.h>
#include <flickr_oauth.h>

#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace app {

    /*
     * OAuth request parameter names
     */
    static const QByteArray OAUTH_PARM_CONSUMER_KEY("oauth_consumer_key");
    static const QByteArray OAUTH_PARM_NONCE("oauth_nonce");
    static const QByteArray OAUTH_PARM_SIGNATURE_METHOD("oauth_signature_method");
    static const QByteArray OAUTH_PARM_SIGNATURE("oauth_signature");
    static const QByteArray OAUTH_PARM_TIMESTAMP("oauth_timestamp");
    static const QByteArray OAUTH_PARM_VERSION("oauth_version");
    static const QByteArray OAUTH_PARM_CALLBACK("oauth_callback");

    /*
     * oob = "Out of Bounds"
     */
    static const QByteArray OAUTH_NO_CALLBACK("oob");
    static const QByteArray OAUTH_VERSION_USED("1.0");

    /*
     * Request Token Url
     */
    static const char* REQUEST_TOKEN_URL = "https://www.flickr.com/services/oauth/request_token";

    FlickrOAuthRequest::FlickrOAuthRequest(const QString& url, QObject* parent) :
    FlickrGetRequest(url)
    {
    }

    FlickrOAuthRequest::~FlickrOAuthRequest()
    {
    }

    const QByteArray& FlickrOAuthRequest::getConsumerKey() const {
        return consumerKey;
    }

    void FlickrOAuthRequest::setConsumerKey(const QByteArray& key) {
        consumerKey = key;
        addEncodedRequestParam(OAUTH_PARM_CONSUMER_KEY, consumerKey); 
    }

    QNetworkReply* FlickrOAuthRequest::send(QNetworkAccessManager& networkAccessMan) {
        addEncodedRequestParam(OAUTH_PARM_NONCE, getNonce());
        addEncodedRequestParam(OAUTH_PARM_SIGNATURE_METHOD, getSignatureMethod()); 
        addEncodedRequestParam(OAUTH_PARM_VERSION, OAUTH_VERSION_USED);
        addEncodedRequestParam(OAUTH_PARM_CALLBACK, OAUTH_NO_CALLBACK);

        // send time = timestamp for this request
        updateTimeStamp();
        addEncodedRequestParam(OAUTH_PARM_TIMESTAMP, getTimeStampString());

        // at this point the signature is generated and added to the request 
        addEncodedRequestParam(OAUTH_PARM_SIGNATURE, getSignature().toPercentEncoding(), false);

        return FlickrGetRequest::send(networkAccessMan);
    }

    FlickrOAuthAuthentication::FlickrOAuthAuthentication(QObject* parent) :
    QObject(parent),
    networkAccessMan(new QNetworkAccessManager(this)),
    state(Initialized)
    {
    }

    FlickrOAuthAuthentication::FlickrOAuthAuthentication(QNetworkAccessManager& networkAccessMan, QObject* parent) :
    QObject(parent),
    networkAccessMan(&networkAccessMan),
    state(Initialized)
    {
    }

    FlickrOAuthAuthentication::~FlickrOAuthAuthentication()
    {
    }

    void FlickrOAuthAuthentication::authenticate() {
        sendRequestTokenRequest();
    }

    void FlickrOAuthAuthentication::sendRequestTokenRequest() {
        Q_ASSERT(state == FlickrOAuthAuthentication::Initialized);
        state = FlickrOAuthAuthentication::GettingRequestToken;
        
        FlickrOAuthRequest oauthRequest(REQUEST_TOKEN_URL);
        oauthRequest.setConsumerKey(FLICKR_API_KEY);
        oauthRequest.setConsumerSecret(FLICKR_API_SECRET);

        flickrResponse = oauthRequest.send(*networkAccessMan);
        qDebug() << "request sent";
        connect(flickrResponse, SIGNAL(finished()), this, SLOT(requestTokenRequestResponseReceived()));
        // might have already received a response
        if(flickrResponse->isFinished()) {
            requestTokenRequestResponseReceived();
        }
    }

    void FlickrOAuthAuthentication::requestTokenRequestResponseReceived() {
        if(flickrResponse->error() == QNetworkReply::NoError) {
            qDebug() << "Success!";
            qDebug() << flickrResponse->url().toString();
        } else {
            qDebug() << "Error!";
        }

        qDebug() << flickrResponse->readAll();
        disconnect(flickrResponse, 0, this, 0);
        flickrResponse->deleteLater();
    }
}
