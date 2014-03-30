#ifndef __FLICKR_REQUEST_H__
#define __FLICKR_REQUEST_H__

/*
 * Base class for requests to Flickr API
 *
 */

#include <QObject>
#include <QByteArray>
#include <QString>
#include <QMap>
#include <QUrl>

class QNetworkAccessManager;
class QNetworkReply;

namespace app {

class FlickrRequestBase : public QObject {
    Q_OBJECT

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
    FlickrRequestBase(const QString&, QObject* parent = 0);
    FlickrRequestBase(const QString&, const QByteArray&, const QByteArray&, QObject* parent = 0);
    virtual ~FlickrRequestBase();

    const QString& getUrl() const;
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

    virtual QNetworkReply* send(QNetworkAccessManager&) = 0;

    protected: // methods
    virtual const char* getHTTPVerb();
    QByteArray generateParamListString(bool);

    private: // methods
    void generateSignature();
    void generateNonce();

    private: // data members
    QString url;
    QByteArray key;
    QByteArray secret;
    QByteArray nonce;
    EncodedRequestParamMap encodedRequestParams;
    TimeStamp timeStamp;

    QByteArray signature;
}; 

/*
 * Flickr requests via HTTP/HTTPS GET
 */
class FlickrGetRequest : public FlickrRequestBase {
    Q_OBJECT

    public:
    FlickrGetRequest(const QString&, QObject* parent = 0);
    FlickrGetRequest(const QString&, const QByteArray&, const QByteArray&, QObject* parent = 0);

    virtual QNetworkReply* send(QNetworkAccessManager&);
};

} // end namespace app

#endif
