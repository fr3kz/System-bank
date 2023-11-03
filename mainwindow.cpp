#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QDebug>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QSettings>
#include <QUrlQuery>
#include "apiservice.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


}

MainWindow::~MainWindow()
{
    delete ui;
}




// strona logowania -> logowanie
void MainWindow::on_pushButton_clicked()
{
    // Pobranie danych z pól interfejsu użytkownika
    QString user_id = ui->user_id_field->text();
    QString password = ui->password_field->text();

    apiservice *api = new apiservice(this);
    QUrl url = api->setUrl("http://127.0.0.1:8000/login/");
    QNetworkRequest request = api->setRequest(url);
    request = api->setHeader(1, "1", request);
    QUrlQuery postData = api->setQueryData();
    postData.addQueryItem("username", user_id);
    postData.addQueryItem("password", password);

    QString a = api->post(postData, request);
    //dodanie sessionid
    QJsonDocument jsonDoc = QJsonDocument::fromJson(a.toUtf8());
    QJsonObject jsonObj = jsonDoc.object();
    //qDebug() << jsonObj;
   // qDebug() << jsonObj["userid"].toInt();
//    QJsonArray jsonArray = jsonDoc.array();
 //   QJsonObject firstObject = jsonArray[0].toObject();
   // qDebug() << firstObject["sessonid"].toString();
    QSettings settings("firma","nienazwany1");
    settings.setValue("sessionid",jsonObj["sessionid"].toString());
    settings.setValue("userid",jsonObj["userid"].toInt());
    settings.setValue("X-CSRFToken",jsonObj["csrf"].toString());



    delete api;
    menu = new Menu(this);
    hide();
    menu->show();
}


void MainWindow::on_pushButton2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

