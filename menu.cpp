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
    ui(new Ui::Menu), apiService(new apiservice(this))
{
    ui->setupUi(this);
    ui->test->setText("IMIE");


    QSettings settings("firma","nienazwany1");
    QString usrid = settings.value("userid").toString();


    QString url = "http://127.0.0.1:8000/user_detail/" + usrid + "/";

    QJsonDocument response = apiService->get_auth(url);
    QJsonObject jobj = response.object();

    ui->test->setText( jobj["username"].toString());

}


Menu::~Menu()
{
    delete ui;
    delete apiService;
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
    QString acc_id = settings.value("account_number").toString();
    // set url
    QString url_ = "http://127.0.0.1:8000/accounts/account_history/" + acc_id + "/";

    QJsonDocument response = apiService->get_auth(url_);
    QJsonArray jsonArray = response.array();

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


}

// konta
void Menu::on_pushButton_3_clicked()
{


    ui->stackedWidget->setCurrentIndex(2);
    QSettings settings("firma", "nienazwany1");
    QString usrid_ = settings.value("userid").toString();


    QString url_ = "http://127.0.0.1:8000/accounts/account_detail/" + usrid_ + "/";
   // qDebug() << url_;

    QJsonDocument response = apiService->get_auth(url_);

    QJsonObject jobj = response.object();

    int balance = jobj["account_balance"].toInt();
    qDebug() << balance;

    ui->label_7->setText(QString::number(jobj["account_balance"].toInt()));


    if(jobj["account_type"].toInt() == 1){
        ui->label_8->setText("Konto osobiste");

    }

    ui->label_9->setText(QString::number(jobj["account_number"].toInt()));

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
    QSettings settings("firma", "nienazwany1");

    QString title = ui->title->text();
    QString accid_owner = settings.value("account_number").toString();
    qDebug() << accid_owner;
    QString amount = ui->amount->text();
    QString accid = ui->acc_id->text();

    QJsonObject transfer;
    transfer["account1_id"] = accid_owner;
    transfer["account2_id"] = accid;
    transfer["amount"] = amount;
    transfer["title"] = title;


    QJsonDocument jsonDoc(transfer);

    QByteArray data = jsonDoc.toJson();

    QJsonDocument response = apiService->post_auth("http://127.0.0.1:8000/accounts/transfer/",data);

    QJsonObject responseObj = response.object();
    qDebug() << responseObj;

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




