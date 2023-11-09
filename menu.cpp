#include "menu.h"
#include "ui_menu.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>
#include <QJsonObject>
#include <QJsonObject>
#include <QUrl>
#include <QDebug>
#include <QMessageBox>
#include <QJsonArray>
#include <QSettings>
#include <QJsonObject>
#include <QJsonDocument>
#include "apiservice.h"


QJsonObject *user = nullptr;
Menu::Menu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Menu)
{
    ui->setupUi(this);
    ui->test->setText("IMIE");

    int usr_id_ = 1;
    QSettings settings("firma","nienazwany1");
    QString usrid = settings.value("userid").toString();
    qDebug() << usrid;
    bool ok;
    usr_id_ = usrid.toInt(&ok);
    qDebug() << usr_id_;
    apiservice *api = new apiservice(this);
    QUrl url = api->setUrl("http://127.0.0.1:8000/user_detail/"+std::to_string(usr_id_)+"/");
    QNetworkRequest request = api->setRequest(url);
    request = api->setHeader(0,"lboqsqnmvqaavx6t839ynr9mq2g2648q",request);

    QString reply = api->get(request);

  //  qDebug() << reply;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(reply.toUtf8());
   // QJsonArray jsonArray = jsonDoc.array();
    //QJsonObject firstObject = jsonArray[0].toObject();
    QJsonObject jobj = jsonDoc.object();
   // ui->test->setText( jobj["username"].toString());
    ui->test->setText( jobj["username"].toString());
    user = &jobj;
    delete api;

}


Menu::~Menu()
{
    delete ui;
}
// Przelewy
void Menu::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

//Historia
void Menu::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    // get user main account number
    //dodanie pozniej pola zeby wybrac ktore konto
    QSettings settings("firma", "nienazwany1");
    std::string acc_id = "111111";
    // set url
    std::string url_ = "http://127.0.0.1:8000/accounts/account_history/" + acc_id + "/";

    qDebug() << url_;
    apiservice *api = new apiservice(this);

    QUrl url = api->setUrl(url_);
    QNetworkRequest request = api->setRequest(url);

    //dodanie HEADER do ogarniecia
    QString sessionId = settings.value("sessionid").toString();
    sessionId = "sessionid="+sessionId;
    request = api->setHeader(1, "1", request);
    QString csrfToken = settings.value("X-CSRFToken").toString();
    request.setRawHeader("X-CSRFToken", csrfToken.toUtf8());
    request.setRawHeader("Cookie", sessionId.toUtf8());
  ///

    QString response = api->get(request);
   // qDebug() << response;
    QJsonDocument jdoc = QJsonDocument::fromJson(response.toUtf8());
    QJsonArray jArray = jdoc.array();
    QJsonArray jsonArray = jdoc.array();




    QList<QMap<QString, QVariant>> transfers;

    // Iteracja po elementach tablicy JSON
    for (const QJsonValue &value : jsonArray) {
        if (value.isObject()) {
            QJsonObject jsonObj = value.toObject();

            // Tworzenie mapy z danymi pojedynczego transferu
            QMap<QString, QVariant> transferData;
            transferData["id"] = jsonObj["id"].toInt();
            transferData["account1_id"] = jsonObj["account1_id"].toInt();
            transferData["account2_id"] = jsonObj["account2_id"].toInt();
            transferData["amount"] = jsonObj["amount"].toInt();
            transferData["date"] = QDateTime::fromString(jsonObj["date"].toString(), Qt::ISODate);

            // Dodanie mapy do listy
            transfers.append(transferData);
        }
    }

    // Wyświetlanie danych
    for (const auto &transferData : transfers) {
        qDebug() << "ID:" << transferData["id"] << "Account1:" << transferData["account1_id"] << "Account2:" << transferData["account2_id"] << "Amount:" << transferData["amount"] << "Date:" << transferData["date"].toDateTime().toString(Qt::ISODate);
    }
    QListWidget *listwidget = ui->listWidget;


    for (const auto &transferData : transfers) {
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(QString("ID: %1, Account1: %2, Account2: %3, Amount: %4, Date: %5")
                          .arg(transferData["id"].toInt())
                          .arg(transferData["account1_id"].toInt())
                          .arg(transferData["account2_id"].toInt())
                          .arg(transferData["amount"].toInt())
                          .arg(transferData["date"].toDateTime().toString(Qt::ISODate)));

        listwidget->addItem(item);
    }

    listwidget->show();
    delete api;

}

// konta
void Menu::on_pushButton_3_clicked()
{


    ui->stackedWidget->setCurrentIndex(2);
    QSettings settings("firma", "nienazwany1");
    QString usrid_ = settings.value("userid").toString();


    std::string url_ = "http://127.0.0.1:8000/accounts/account_detail/" + usrid_.toStdString() + "/";
   // qDebug() << url_;

    apiservice *api = new apiservice(this);
    QUrl url = api->setUrl(url_);
    QNetworkRequest request = api->setRequest(url);


    QString sessionId = settings.value("sessionid").toString();
    sessionId = "sessionid="+sessionId;
    request = api->setHeader(1, "1", request);
    QString csrfToken = settings.value("X-CSRFToken").toString();
    request.setRawHeader("X-CSRFToken", csrfToken.toUtf8());
    request.setRawHeader("Cookie", sessionId.toUtf8());

    QString response = api->get(request);
    qDebug() << response;

    QJsonDocument jdoc = QJsonDocument::fromJson(response.toUtf8());

    QJsonObject jobj = jdoc.object();

    int balance = jobj["account_balance"].toInt();
    qDebug() << balance;

    ui->label_7->setText(QString::number(jobj["account_balance"].toInt()));

    ui->label_8->setText(QString::number(jobj["account_type"].toInt()));

    ui->label_9->setText(QString::number(jobj["account_number"].toInt()));

    delete api;
}

//wyloguj
void Menu::on_pushButton_4_clicked()
{
    QCoreApplication::quit();
}


void Menu::on_pushButton_5_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

//transfer
void Menu::on_commandLinkButton_clicked()

{
  /*  QSettings settings("firma", "nienazwany1");

    QString title = ui->title->text();
    QString amount = ui->amount->text();
    QString accid = ui->acc_id->text();

    QString sessionid = settings.value("sessionid").toString();
    QByteArray sessionidbyte = sessionid.toUtf8();
     apiservice *api1 = new apiservice(this);

    QString csrf = api1->get_csrf();
    QByteArray csrfByteArray = csrf.toUtf8();

    QUrl url = api1->setUrl("http://127.0.0.1:8000/accounts/transfer/");
    QNetworkRequest request = api1->setRequest(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader("X-CSRFToken",csrfByteArray);
    QUrlQuery query = api1->setQueryData();
    query.addQueryItem("amount", "1");
    query.addQueryItem("accound1_id", "111111");
    query.addQueryItem("accound2_id","333333");
    query.addQueryItem("title","title");

    QString reply = api1->post(query,request);

    QJsonDocument jsonDoc = QJsonDocument::fromJson(reply.toUtf8());
    QJsonObject jsonObj = jsonDoc.object();
    qDebug() << jsonObj;

    delete api1;

*/

    QString title = ui->title->text();
    std::string title_str = title.toStdString();
    QString amount = ui->amount->text();
    QString accid = ui->acc_id->text();
    QUrlQuery postData;
    postData.addQueryItem("account1_id", "111111");
    postData.addQueryItem("account2_id", accid);
    postData.addQueryItem("amount",amount);
    postData.addQueryItem("title",title);

    // Utwórz obiekt managera i żądanie
    QNetworkAccessManager *manager = new QNetworkAccessManager;
    QNetworkRequest request(QUrl("http://127.0.0.1:8000/accounts/transfer/"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    // Wysyłanie żądania POST
    QNetworkReply *reply = manager->post(request, postData.toString(QUrl::FullyEncoded).toUtf8());

    // Oczekiwanie na zakończenie żądania
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    // Obsługa odpowiedzi
    QString data;

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        data = QString(responseData);
        QJsonDocument jdoc = QJsonDocument::fromJson(data.toUtf8());
        QJsonObject jdata = jdoc.object();
        qDebug() << jdata;
    } else {
        qDebug() << "Error: " << reply->errorString();
    }

    // Usuń obiekt odpowiedzi i managera
    reply->deleteLater();
    manager->deleteLater();






    ui->stackedWidget->setCurrentIndex(0);

}

//powrot
void Menu::on_pushButton_6_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void Menu::on_pushButton_7_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}




