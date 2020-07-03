#include "login.h"
#include "ui_login.h"
#include<iostream>



Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);


    if(!connOpen()){
        ui->label->setText("Database connection failed!");
    }else{
       // ui->label->setText("Database Connected");
    }
}

Login::~Login()
{
    delete ui;
}



void Login::on_pushButton_login_clicked()
{
    QString username,password;
    username = ui->lineEdit_username->text();
    password = ui->lineEdit_password->text();

    if(!connOpen()){
        qDebug()<<"failed to open database";
        return;
    }

    connOpen();
    QSqlQuery qry;
    qry.prepare("select * from users where username='"+username+"' and password='"+password+"'");
    if(qry.exec()){
       int count = 0;
        while(qry.next()){
            count++;
           typeofuser = qry.value(6).toString();
   }
        if(count == 1){
         ui->label->setText("Username and password are correct");
         connClose();
         this->hide();
          MainPage mainpage;
         /* if(typeofuser == "user"){
          mainpage.buttonEnabled = false;
          }*/
          mainpage.setModal(true);
          mainpage.exec();

        }
        if(count < 1){
            ui->label->setText("Wrong username and Password");
        }
    }
}

void Login::newWindow()
{


}


