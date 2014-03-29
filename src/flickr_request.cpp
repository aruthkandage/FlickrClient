#include <flickr_request.h>
#include <hmac.h>

#include <QDateTime>

namespace app {

/* 
 * The nonce (for oauth) is 32 random bytes which are base64 encoded 
 */
static const unsigned int NONCE_BYTES = 32;

FlickrRequestBase::FlickrRequestBase(const QString& url) :
url(url)
{
}

FlickrRequestBase::FlickrRequestBase(const QString& url, const QByteArray& key, const QByteArray& secret) :
url(url),
key(key),
secret(secret)
{
}

FlickrRequestBase::~FlickrRequestBase()
{
}

const QByteArray& FlickrRequestBase::getKey() const {
    return key;
}

const QByteArray& FlickrRequestBase::getSecret() const {
    return secret;
}

void FlickrRequestBase::setKey(const QByteArray& key) {
    this->key = key;
}

void FlickrRequestBase::setSecret(const QByteArray& secret) {
    this->secret = secret;
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

/*
 * Required for generation of the signature
 * default to GET
 */
const char* FlickrRequestBase::getHTTPVerb() {
    return "GET";
}

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

    // put the key and secret together to build the signing key
    signingKey.append(key);
    signingKey.push_back('&');
    signingKey.append(secret);

    signature = HMAC::sha1(signingKey, signatureBase).toBase64();
}

/*
 * Generate a random string of NONCE_BYTES, then base64 encode it
 */
void FlickrRequestBase::generateNonce() {
    QByteArray nonceBytes(NONCE_BYTES, 0);

    for(int i=0; i < NONCE_BYTES; i++) {
        unsigned char byte = (unsigned char) qrand();
        nonceBytes[i] = byte;
    }

    nonce = nonceBytes.toBase64();
}

} // end namespace app
