#ifndef APISERVICE_H
#define APISERVICE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QUrlQuery>
#include <QByteArray>
#include <QNetworkReply>

class apiservice : public QObject
{
    Q_OBJECT

public:
    apiservice(QObject *parent = nullptr);
    QNetworkRequest setRequest(QUrl url);
    QNetworkRequest setHeader(int i, const std::string &header,QNetworkRequest request);
    QUrl setUrl(std::string url_string);
    QUrlQuery setQueryData();
    QString post(QUrlQuery postData, QNetworkRequest request);
    QString get( QNetworkRequest request);
    QString get_csrf();
private:
    QNetworkAccessManager *manager;
};

#endif // APISERVICE_H
