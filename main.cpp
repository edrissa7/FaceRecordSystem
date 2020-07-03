//#include "users.h"
#include "login.h"


#include <QApplication>
#include<iostream>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QtDebug>
#include<QFileInfo>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Login w;
    w.show();

    return a.exec();

}
