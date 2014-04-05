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
#include <QUrlQuery>

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
    static const QByteArray OAUTH_PARM_TOKEN("oauth_token");
    static const QByteArray OAUTH_PARM_TOKEN_SECRET("oauth_token_secret");
    static const QByteArray OAUTH_PARM_VERIFIER("oauth_verifier");

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

    FlickrOAuthManager::FlickrOAuthManager(QObject* parent) :
    QObject(parent),
    networkAccessMan(new QNetworkAccessManager(this)),
    state(Initialized)
    {
    }

    FlickrOAuthManager::FlickrOAuthManager(QNetworkAccessManager& networkAccessMan, QObject* parent) :
    QObject(parent),
    networkAccessMan(&networkAccessMan),
    state(Initialized)
    {
    }

    FlickrOAuthManager::~FlickrOAuthManager()
    {
    }

    void FlickrOAuthManager::setState(FlickrOAuthManager::State nextState) {
        state = nextState;
    }

    FlickrOAuthManager::State FlickrOAuthManager::getState() const {
        return state;
    }

    void FlickrOAuthManager::authenticate() {
        // Kick off with the first authentication step
        requestRequestToken();
    }

    void FlickrOAuthManager::destroyResponse() {
        if(flickrResponse != 0) {
            flickrResponse->disconnect(this);
            flickrResponse->deleteLater();
            flickrResponse = 0;
        }
    }

    /*
     * First authentication step -- getting the request token
     */
    void FlickrOAuthManager::requestRequestToken() {
        Q_ASSERT(state == FlickrOAuthManager::Initialized);
        setState(FlickrOAuthManager::GettingRequestToken);
        
        FlickrOAuthRequest oauthRequest(REQUEST_TOKEN_URL);
        oauthRequest.setConsumerKey(FLICKR_API_KEY);
        oauthRequest.setConsumerSecret(FLICKR_API_SECRET);

        flickrResponse = oauthRequest.send(*networkAccessMan);
        flickrResponse->setParent(this);
        connect(flickrResponse, SIGNAL(finished()), this, SLOT(requestTokenResponseReceived()));

        // might have already received a response
        if(flickrResponse->isFinished()) {
            requestTokenResponseReceived();
        }
    }

    /*
     * The request token and the token secret are in the response
     */ 
    void FlickrOAuthManager::extractRequestTokenAndSecret() {
        QUrlQuery responseValues = QUrlQuery(flickrResponse->readAll());

        token = responseValues.queryItemValue(OAUTH_PARM_TOKEN, QUrl::FullyEncoded).toUtf8();
        tokenSecret = responseValues.queryItemValue(OAUTH_PARM_TOKEN_SECRET, QUrl::FullyEncoded).toUtf8();

        qDebug() << __FILE__ << ":" << __LINE__ << "Token: " << token;
        qDebug() << __FILE__ << ":" << __LINE__ << "Secret: " << tokenSecret;
    }

    void FlickrOAuthManager::requestTokenResponseReceived() {
        if(flickrResponse->error() == QNetworkReply::NoError) {
            extractRequestTokenAndSecret();
            destroyResponse();
            setState(FlickrOAuthManager::RequestTokenReceived);
        } else {
            qDebug() << __FILE__ << ":" << __LINE__ << "Error occurred while requesting request token" << token;

            destroyResponse();
            setState(FlickrOAuthManager::Error);

            // TODO: process the error and send off with a reason code
            emit error();
        }
    }
}
