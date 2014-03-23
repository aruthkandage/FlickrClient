#ifndef __FLICKR_REQUEST_H__
#define __FLICKR_REQUEST_H__

/*
 * Base class for requests to Flickr API
 *
 */

#include <QByteArray>
#include <QString>
#include <QMap>
#include <QUrl>

namespace app {

class FlickrRequest {
    public:
    enum HTTPVerb {
        GET,
        POST
    };

    FlickrRequest(const QString&);
    FlickrRequest(const QString&, const QByteArray&, const QByteArray&);
    FlickrRequest(const QString&, const QByteArray&, const QByteArray&, HTTPVerb);
    virtual ~FlickrRequest();

    void setHTTPVerb(HTTPVerb verb);
    HTTPVerb getHTTPVerb() const;

    const QByteArray& getKey() const;
    const QByteArray& getSecret() const;

    void setKey(const QByteArray& key);
    void setSecret(const QByteArray& secret);

    void addRequestParam(const QString&, const QString&, bool includeInSignature = true, bool percentEncodeParamName = false);
    bool removeRequestParam(const QString&);
    const QByteArray& getSignature(bool regenerate = false);
    QUrl toUrl();

    protected:
    struct EncodedRequestParam {
        QByteArray value;
        bool includeInSignature;
    }; 

    virtual bool signatureIsValid();
    
    private: // methods
    void appendParamList(QByteArray&, bool onlySignatureParams = false);
    void generateQuery();
    void generateSignature();

    private: // data members
    HTTPVerb httpVerb;
    QString url;
    QByteArray query;
    QByteArray key;
    QByteArray secret;
    QMap<QByteArray, EncodedRequestParam> encodedRequestParams;

    QByteArray signature;
}; 

} // end namespace app

#endif
