#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QDialog>
#include "menu.h"
#include "users.h"
#include "imagesave.h"

namespace Ui {
class MainPage;
}

class MainPage : public QDialog
{
    Q_OBJECT

public:
     bool buttonEnabled;
    explicit MainPage(QWidget *parent = nullptr);
    ~MainPage();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_logout_clicked();

private:
    Ui::MainPage *ui;
    Menu *detection;
    Users *userManage;

};

#endif // MAINPAGE_H
