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

class FlickrRequestBase {
    protected: // types
    struct EncodedRequestParam {
        QByteArray value;
        bool includeInSignature;
    }; 

    typedef QMap<QByteArray, EncodedRequestParam> EncodedRequestParamMap; 
    typedef QMap<QByteArray, EncodedRequestParam>::iterator EncodedRequestParamMapIter;

    public: // types
    typedef qint64 TimeStamp;

    public: // methods
    FlickrRequestBase(const QString&);
    FlickrRequestBase(const QString&, const QByteArray&, const QByteArray&);
    virtual ~FlickrRequestBase();

    const QByteArray& getKey() const;
    const QByteArray& getSecret() const;

    void setKey(const QByteArray& key);
    void setSecret(const QByteArray& secret);

    void addRequestParam(const QString&, const QString&, bool includeInSignature = true);
    void addEncodedRequestParam(const QByteArray&, const QByteArray&, bool includeInSignature = true);
    bool removeRequestParam(const QString&);
    bool removeEncodedRequestParam(const QByteArray&);
    void clearSignature();
    const QByteArray& getSignature(bool regenerate = false);
    const QByteArray& getNonce(bool regenerate = false);

    void updateTimeStamp();
    TimeStamp getTimeStamp() const;

    protected: // methods
    virtual const char* getHTTPVerb();

    private: // methods
    void generateSignature();
    void generateNonce();
    QByteArray generateParamListString(bool);

    private: // data members
    QString url;
    QByteArray key;
    QByteArray secret;
    QByteArray nonce;
    EncodedRequestParamMap encodedRequestParams;
    TimeStamp timeStamp;

    QByteArray signature;
}; 

} // end namespace app

#endif
