#ifndef __FLICKR_REQUEST_H__
#define __FLICKR_REQUEST_H__

/*
 * Base class for requests to Flickr API
 *
 */

#include <QString>
#include <QMap>
#include <QNetworkRequest>

class FlickrRequest : public QNetworkRequest {
    public:
    FlickrRequest(QString&);
    FlickrRequest(QString&, HTTPVerb);

    enum HTTPVerb {
        GET,
        POST
    };

    public void setHTTPVerb(HTTPVerb verb) {
        this->httpVerb = verb;
    }
    public HTTPVerb getHTTPVerb() const {
        return httpVerb;
    }
    public void setUrl(const QString& url) {
        this->url = url;
    }
    public const QString& getUrl() const {
        return url;
    }

    public const QString& getSignature();

    protected:
    struct RequestParam {
        QString value;
        bool includeInSignature;
    };    

    private:
    HTTPVerb httpVerb;
    QString url;
    QMap<QString, RequestParam> requestParams;

    bool signatureGenerated;
    qint64 signatureTimestamp;
    QString signature;
}; 

#endif
