#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QDebug>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QSettings>
#include <QUrlQuery>
#include "apiservice.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), apiService(new apiservice(this))
{
    ui->setupUi(this);


}

MainWindow::~MainWindow()
{
    delete ui;
    delete apiService;
}




// strona logowania -> logowanie
void MainWindow::on_pushButton_clicked()
{
    // Pobranie danych z pól interfejsu użytkownika
    QString user_id = ui->user_id_field->text();
    QString password = ui->password_field->text();

    QJsonObject user;
    user["username"] = user_id;
    user["password"] = password;

    if(user_id == "" || password == "")
    {
        QMessageBox::critical(this,"Error","Wypełnij wszystkie pola");
        return;
    }

    QByteArray data = QJsonDocument(user).toJson();


    QJsonDocument response = apiService->post("http://127.0.0.1:8000/login/",data);
    QJsonObject responseObj = response.object();
    qDebug() << responseObj;

    if(responseObj["value"].toString() != "")
    {
        qDebug() << responseObj["error"].toString();
        QJsonObject errorobj = responseObj["error"].toObject();
        QJsonArray valueArray = errorobj["value"].toArray();

        QMessageBox::critical(this,"Error",valueArray[0].toString());
        return;
    }else{

        QSettings settings("firma","nienazwany1");
        settings.setValue("csrf",responseObj["csrf"].toString());
        settings.setValue("sessionid",responseObj["sessionid"].toString());
        settings.setValue("userid",responseObj["userid"].toInt());
        settings.setValue("account_number",responseObj["account_number"].toInt());


        Menu *menu = new Menu(this);
        hide();
        menu->show();
        return;
    }

    if (responseObj["error"].toString() != "") {
        QMessageBox::critical(this, "Błąd", responseObj["value"].toString());
        return;
    }

}


void MainWindow::on_pushButton2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

// rejestrowanie
void MainWindow::on_pushButton_5_clicked()
{
    QString username,email,password,firstname,lastname;

    //wypelnienie danych

    firstname = ui->lineEdit->text();
    lastname = ui->lineEdit_2->text();
    username = ui->user_id_field_5->text();
    password = ui->password_field_2->text();

    QJsonObject user;
    user["username"] = username;
    user["password"] = password;
    user["firstname"] = firstname;
    user["lastname"] = lastname;

    if(username == "" || password == "" || firstname == "" || lastname == "")
    {
        QMessageBox::critical(this,"Error","Wypełnij wszystkie pola");
        return;
    }

    QJsonDocument jsonDoc(user);

    QByteArray data = jsonDoc.toJson();

    QJsonDocument response = apiService->post("http://127.0.0.1:8000/register/",data);

    QJsonObject responseObj = response.object();

       if(responseObj["value"].toString() != "")
    {
        qDebug() << responseObj["error"].toString();
        QJsonObject errorobj = responseObj["error"].toObject();
        QJsonArray valueArray = errorobj["value"].toArray();

        QMessageBox::critical(this,"Error",valueArray[0].toString());
        return;
    }else{
        qDebug() << responseObj["account_number"].toInt();
        QSettings settings("firma","nienazwany1");
        settings.setValue("csrf",responseObj["csrf"].toString());
        settings.setValue("sessionid",responseObj["sessionid"].toString());
        settings.setValue("userid",responseObj["userid"].toInt());
        settings.setValue("account_number",responseObj["account_number"].toInt());


        Menu *menu = new Menu(this);
        hide();
        menu->show();
        return;
    }

    if (responseObj["error"].toString() != "") {
        QMessageBox::critical(this, "Błąd", responseObj["value"].toString());
        return;
    }

    QSettings settings("firma","nienazwany1");
    qDebug() << responseObj;
    settings.setValue("sessionid",responseObj["sessionid"].toString());
    settings.setValue("userid",responseObj["userid"].toInt());
    settings.setValue("X-CSRFToken",responseObj["csrf"].toString());

    menu = new Menu(this);
    menu->show();



}

