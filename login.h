#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QtDebug>
#include<QFileInfo>

#include<iostream>

#include "mainpage.h"

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
       QString typeofuser;
       void connClose(){
        mydb.close();
        mydb.removeDatabase(QSqlDatabase::defaultConnection);
    }
    bool connOpen(){
        mydb=QSqlDatabase::addDatabase("QSQLITE");
        mydb.setDatabaseName("C:/Users/user/Desktop/sqlite-amalgamation-3270200/systemDB.db");
        if(!mydb.open()){
           qDebug()<<("Database connection failed!");
           return false;
        }else{
          qDebug()<<("Database Connected");
          return true;
        }

    }
    QSqlDatabase mydb;

public:
    explicit Login(QWidget *parent = nullptr);
    void newWindow();
    ~Login();

private slots:

    void on_pushButton_login_clicked();

   private:
    Ui::Login *ui;
    MainPage *mainpage;
};

#endif // LOGIN_H
