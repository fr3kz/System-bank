#ifndef APISERVICE_H
#define APISERVICE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QUrlQuery>
#include <QByteArray>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QByteArray>

class apiservice : public QObject
{
    Q_OBJECT
public:
    explicit apiservice(QObject *parent = nullptr);
    QJsonDocument post(QString url_,QByteArray data);
    QJsonDocument post_auth(QString url_,QByteArray data);
    QJsonDocument get(QString url_);
    QJsonDocument get_auth(QString url_);
    QString get_csrf();


signals:

};

#endif // APISERVICE_H
