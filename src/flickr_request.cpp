#include <flickr_request.h>
#include <hmac.h>

namespace app {

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

    // signature must be regenerated when needed
    if(!signature.isEmpty()) {
        signature.clear();
    }
}

bool FlickrRequestBase::removeRequestParam(const QString& paramName) {
    QByteArray paramNameEncoded = paramName.toUtf8().toPercentEncoding();
    
    int numRemoved = encodedRequestParams.remove(paramNameEncoded);

    // signature must be regenerated when needed
    if(!signature.isEmpty()) {
        signature.clear();
    }

    return numRemoved == 1;
}

const QByteArray& FlickrRequestBase::getSignature(bool regenerate) {
    if(signature.isEmpty() || regenerate) {
        generateSignature();
    }

    return signature;
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
const char* getHTTPVerb() {
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

} // end namespace app
