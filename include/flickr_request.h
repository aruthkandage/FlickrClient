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
    public:
    FlickrRequestBase(const QString&);
    FlickrRequestBase(const QString&, const QByteArray&, const QByteArray&);
    virtual ~FlickrRequestBase();

    const QByteArray& getKey() const;
    const QByteArray& getSecret() const;

    void setKey(const QByteArray& key);
    void setSecret(const QByteArray& secret);

    void addRequestParam(const QString&, const QString&, bool includeInSignature = true);
    bool removeRequestParam(const QString&);
    const QByteArray& getSignature(bool regenerate = false);

    protected:
    struct EncodedRequestParam {
        QByteArray value;
        bool includeInSignature;
    }; 

    virtual const char* getHTTPVerb();

    private: // methods
    void generateSignature();
    QByteArray generateParamListString(bool);

    private: // data members
    QString url;
    QByteArray key;
    QByteArray secret;
    QMap<QByteArray, EncodedRequestParam> encodedRequestParams;

    QByteArray signature;
}; 

} // end namespace app

#endif
