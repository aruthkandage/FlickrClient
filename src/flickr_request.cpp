#include <flickr_request.h>
#include <hmac.h>

namespace app {

FlickrRequest::FlickrRequest(const QString& url) :
url(url)
{
}

FlickrRequest::FlickrRequest(const QString& url, const QByteArray& key, const QByteArray& secret) :
url(url),
key(key),
secret(secret)
{
}

FlickrRequest::FlickrRequest(const QString& url, const QByteArray& key, const QByteArray& secret, FlickrRequest::HTTPVerb httpVerb) :
url(url),
key(key),
secret(secret),
httpVerb(httpVerb)
{
}

FlickrRequest::~FlickrRequest()
{
}

void FlickrRequest::setHTTPVerb(HTTPVerb verb) {
    this->httpVerb = verb;
}

FlickrRequest::HTTPVerb FlickrRequest::getHTTPVerb() const {
    return httpVerb;
}

const QByteArray& FlickrRequest::getKey() const {
    return key;
}

const QByteArray& FlickrRequest::getSecret() const {
    return secret;
}

void FlickrRequest::setKey(const QByteArray& key) {
    this->key = key;
}

void FlickrRequest::setSecret(const QByteArray& secret) {
    this->secret = secret;
}

void FlickrRequest::addRequestParam(const QString& paramName, const QString& paramValue, bool includeInSignature, bool percentEncodeParamName) {
    EncodedRequestParam encodedRequestParam;
    // Note: this is a small optimization in case you know percent encoding is not required
    // for the given param name -- param names should always be valid percent encoded strings
    QByteArray paramNameEncoded = percentEncodeParamName ? paramName.toUtf8().toPercentEncoding() : paramName.toUtf8();

    encodedRequestParam.value = paramValue.toUtf8().toPercentEncoding();
    encodedRequestParam.includeInSignature = includeInSignature;

    encodedRequestParams.insert(paramNameEncoded, encodedRequestParam);

    query.clear();
    signature.clear();
}

bool FlickrRequest::removeRequestParam(const QString& paramName) {
    QByteArray paramNameEncoded = paramName.toUtf8().toPercentEncoding();
    
    int numRemoved = encodedRequestParams.remove(paramNameEncoded);

    query.clear();
    signature.clear();

    return numRemoved == 1;
}

const QByteArray& FlickrRequest::getSignature(bool regenerate) {
    if(signature.isEmpty() || regenerate) {
        generateSignature();
    }

    return signature;
}

void FlickrRequest::appendParamList(QByteArray& queryRef, bool onlySignatureParams) {
    for(QMap<QByteArray, EncodedRequestParam>::iterator param = encodedRequestParams.begin();
        param != encodedRequestParams.end();
        param++) {

        EncodedRequestParam encodedRequestParam = param.value();

        if(!onlySignatureParams || encodedRequestParam.includeInSignature) {
            queryRef.append(param.key());
            queryRef.push_back('=');
            queryRef.append(encodedRequestParam.value);
        }
        
        if(param + 1 != encodedRequestParams.end()) {
            queryRef.push_back('&');
        }
    }
}

void FlickrRequest::generateQuery() {
    if(!query.isEmpty()) query.clear();

    query.push_back('?');
    appendParamList(query);
}

void FlickrRequest::generateSignature() {
    QByteArray signingKey;
    QByteArray signatureBase;
    QByteArray parameterString;

    // start with the HTTP verb
    switch(httpVerb) {
        case GET:
            signatureBase.append("GET");
            break;
        case POST:
            signatureBase.append("POST");
            break;
        default:
            signatureBase.append("GET");
            break;
    }
    signatureBase.push_back('&');

    // then add the percent encoded url
    signatureBase.append(url.toUtf8().toPercentEncoding());
    signatureBase.push_back('&');

    // generate the parameter string
    appendParamList(parameterString, /* onlySignatureParams = */ true);
    signatureBase.append(parameterString.toPercentEncoding());

    // put the key and secret together to build the signing key
    signingKey.append(key);
    signingKey.push_back('&');
    signingKey.append(secret);

    signature = HMAC::sha1(signingKey, signatureBase);
}

} // end namespace app
