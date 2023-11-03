#include "apiservice.h"

#include <QEventLoop>

apiservice::apiservice(QObject *parent) : QObject(parent) {
    manager = new QNetworkAccessManager(this);
}

QNetworkRequest apiservice::setRequest(QUrl url) {
    QNetworkRequest request(url);
    return request;
}

QNetworkRequest apiservice::setHeader(int i, const std::string &header,QNetworkRequest request) {

    if(i==1){
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    }else{
        request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");

        request.setRawHeader("Cookie", "sessionid=" + QString::fromStdString(header).toUtf8());
    }
    return request;
    }

QUrl apiservice::setUrl(std::string url_string) {
    QUrl url(QString::fromStdString(url_string));
    return url;
}

QUrlQuery apiservice::setQueryData() {
    QUrlQuery postData;
    return postData;
}

QString apiservice::post(QUrlQuery postData, QNetworkRequest request) {
    QByteArray postBody = postData.toString(QUrl::FullyEncoded).toUtf8();
    QNetworkReply *reply = manager->post(request, postBody);

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QString data;

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray data_json = reply->readAll();
        data = QString(data_json);
       // qDebug() << data;
    } else {
        qDebug() << reply->errorString();
    }

    reply->deleteLater();
    return data;
}

// Dodanie funkcji GET
QString apiservice::get( QNetworkRequest request){

    QNetworkReply *reply = manager->get(request);
    QEventLoop loop;
    connect(reply,&QNetworkReply::finished,&loop,&QEventLoop::quit);
    loop.exec();

    QString data;
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray data_json = reply->readAll();
        data = QString(data_json);
        // qDebug() << data;
    } else {
        qDebug() << reply->errorString();
    }
    return data;
    reply->deleteLater();

}
