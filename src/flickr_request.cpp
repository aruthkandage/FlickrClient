#include <flickr_request.h>
#include <hmac.h>

#include <QDateTime>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

namespace app {

/*
 * Most Flickr requests are GET requests
 */
static const char* DEFAULT_HTTP_VERB = "GET";

/*
 * Generate signatures using HMAC-SHA1 encryption
 */
static const char* DEFAULT_SIGNATURE_METHOD = "HMAC-SHA1";


/*
 * Eligible characters for the nonce (alphanumeric)
 */
static const char NONCE_CHARS[] = {
'a','b','c','d','e','f','g','h','i','j','k','l',
'm','n','o','p','q','r','s','t','u','v','w','x',
'y','z','A','B','C','D','E','F','G','H','I','J',
'K','L','M','N','O','P','Q','R','S','T','U','V',
'W','X','Y','Z','0','1','2','3','4','5','6','7',
'8','9'};

static const unsigned int NUM_NONCE_CHARS = 62;

/* 
 * Length of the nonce (for oauth) 
 */
static const unsigned int NONCE_LENGTH = 32;

FlickrRequestBase::FlickrRequestBase(const QString& url, QObject* parent) :
QObject(parent),
url(url)
{
}

FlickrRequestBase::FlickrRequestBase(const QString& url, const QByteArray& consumerSecret, const QByteArray& tokenSecret, QObject* parent) :
QObject(parent),
url(url),
consumerSecret(consumerSecret),
tokenSecret(tokenSecret)
{
}

FlickrRequestBase::~FlickrRequestBase()
{
}

const QString& FlickrRequestBase::getUrl() const {
    return url;
}

const QByteArray& FlickrRequestBase::getConsumerSecret() const {
    return consumerSecret;
}

const QByteArray& FlickrRequestBase::getTokenSecret() const {
    return tokenSecret;
}

void FlickrRequestBase::setConsumerSecret(const QByteArray& secret) {
    consumerSecret = secret;
}

void FlickrRequestBase::setTokenSecret(const QByteArray& secret) {
    tokenSecret = secret;
}

void FlickrRequestBase::addRequestParam(const QString& paramName, const QString& paramValue, bool includeInSignature) {
    EncodedRequestParam encodedRequestParam;
    QByteArray paramNameEncoded = paramName.toUtf8().toPercentEncoding();

    encodedRequestParam.value = paramValue.toUtf8().toPercentEncoding();
    encodedRequestParam.includeInSignature = includeInSignature;

    encodedRequestParams.insert(paramNameEncoded, encodedRequestParam);
    if(includeInSignature) {
        clearSignature();
    }
}

void FlickrRequestBase::addEncodedRequestParam(const QByteArray& paramName, const QByteArray& paramValue, bool includeInSignature) {
    EncodedRequestParam encodedRequestParam;

    encodedRequestParam.value = paramValue;
    encodedRequestParam.includeInSignature = includeInSignature;

    encodedRequestParams.insert(paramName, encodedRequestParam);
    if(includeInSignature) {
        clearSignature();
    }
}

bool FlickrRequestBase::removeEncodedRequestParam(const QByteArray& paramNameEncoded) {
    EncodedRequestParamMapIter iter = encodedRequestParams.find(paramNameEncoded);

    if(iter == encodedRequestParams.end()) {
        return false;
    }

    EncodedRequestParam& encodedRequestParam = *iter;
    encodedRequestParams.remove(paramNameEncoded);

    if(encodedRequestParam.includeInSignature) {
        clearSignature();
    }

    return true;
}

bool FlickrRequestBase::removeRequestParam(const QString& paramName) {
    removeEncodedRequestParam(paramName.toUtf8().toBase64());
}

void FlickrRequestBase::clearSignature() {
    // signature must be regenerated when needed
    if(!signature.isEmpty()) {
        signature.clear();
    }
}

const QByteArray& FlickrRequestBase::getSignature(bool regenerate) {
    if(signature.isEmpty() || regenerate) {
        generateSignature();
    }

    return signature;
}

const QByteArray& FlickrRequestBase::getNonce(bool regenerate) {
    if(signature.isEmpty() || regenerate) {
        generateNonce();
    }

    return nonce;
}

void FlickrRequestBase::updateTimeStamp() {
    timeStamp = QDateTime::currentMSecsSinceEpoch();
    clearSignature();
}

FlickrRequestBase::TimeStamp FlickrRequestBase::getTimeStamp() const {
    return timeStamp;
}

/*
 * Generate a string from the parameters as such
 * param_1=value_1&param_2=value_2&...&param_n=value_n
 */
QByteArray FlickrRequestBase::generateParamListString(bool onlySignatureParams) {
    QByteArray paramList;

    for(QMap<QByteArray, EncodedRequestParam>::iterator param = encodedRequestParams.begin();
        param != encodedRequestParams.end();
        param++) {

        EncodedRequestParam encodedRequestParam = param.value();

        if(!onlySignatureParams || encodedRequestParam.includeInSignature) {
            paramList.append(param.key());
            paramList.push_back('=');
            paramList.append(encodedRequestParam.value);
        }
        
        if(param + 1 != encodedRequestParams.end()) {
            paramList.push_back('&');
        }
    }

    return paramList;
}

const char* FlickrRequestBase::getSignatureMethod() const {
    return DEFAULT_SIGNATURE_METHOD;
}

/*
 * Required for generation of the signature
 * default to GET
 */
const char* FlickrRequestBase::getHTTPVerb() const {
    return DEFAULT_HTTP_VERB;
}

/*
 * Generates a HMAC-SHA1 signature according to OAuth spec
 */
void FlickrRequestBase::generateSignature() {
    QByteArray signingKey;
    QByteArray signatureBase;
    QByteArray parameterString;

    // start with the HTTP verb
    signatureBase.append(getHTTPVerb());
    signatureBase.push_back('&');

    // then add the percent encoded url
    signatureBase.append(url.toUtf8().toPercentEncoding());
    signatureBase.push_back('&');

    // generate the parameter string
    signatureBase.append(generateParamListString(/* onlySignatureParams= */ true));

    // put the consumer secret and token secret together to build the signing key
    signingKey.append(consumerSecret);
    signingKey.push_back('&');
    signingKey.append(tokenSecret);

    signature = HMAC::sha1(signingKey, signatureBase).toBase64();
}

/*
 * Generate a random string of NONCE_BYTES, then base64 encode it
 */
void FlickrRequestBase::generateNonce() {
    QByteArray nonceBytes(NONCE_LENGTH, 0);

    for(int i=0; i < NONCE_LENGTH; i++) {
        char byte = NONCE_CHARS[((unsigned int) qrand()) % NUM_NONCE_CHARS]; 
        nonceBytes[i] = byte;
    }

    nonce = nonceBytes;
}

FlickrGetRequest::FlickrGetRequest(const QString& url, QObject* parent) :
FlickrRequestBase(url, parent)
{
}

FlickrGetRequest::FlickrGetRequest(const QString& url, const QByteArray& consumerSecret, const QByteArray& tokenSecret, QObject* parent) :
FlickrRequestBase(url, consumerSecret, tokenSecret, parent)
{
}

QNetworkReply* FlickrGetRequest::send(QNetworkAccessManager& networkAccessMan) {
    QUrl url(getUrl());    

    // set the query string
    QByteArray queryString;
    queryString.push_back('?');
    queryString.append(generateParamListString(/* onlySignatureParams = */ false));
    
    url.setQuery(queryString, QUrl::StrictMode);

    if(url.isValid()) {
        QNetworkRequest request(url);
        // this object can be accessed from the QNetworkReply
        request.setOriginatingObject(this);
   
        return networkAccessMan.get(request); 
    }

    return 0;
}

} // end namespace app
