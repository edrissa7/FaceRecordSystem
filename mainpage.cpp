#include "mainpage.h"
#include "ui_mainpage.h"
#include "login.h"

MainPage::MainPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainPage)
{

    ui->setupUi(this);

      //ui->pushButton_2->setEnabled(buttonEnabled);
   // Login login;
   //  QString *d = &login.typeofuser;

   // cout << *d->toUtf8().constData() << "fff" << endl;

    }

MainPage::~MainPage()
{
    delete ui;

}

void MainPage::on_pushButton_clicked()
{
 detection = new Menu(this);
 detection->show();
  //detection.setWindowModality(Qt::ApplicationModal);
  //detection.show();
}

void MainPage::on_pushButton_2_clicked()
{
    userManage = new Users(this);
    userManage->show();
}

void MainPage::on_pushButton_logout_clicked()
{
    this->close();
     Login login;
       login.exec();
}
