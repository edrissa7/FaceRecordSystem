#ifndef USERS_H
#define USERS_H

#include <QMainWindow>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QMessageBox>
#include<QDate>


namespace Ui {
class Users;
}

class Users : public QMainWindow
{
    Q_OBJECT

public:
    explicit Users(QWidget *parent = nullptr);
    void loadTable();
    ~Users();


private slots:
    void on_pushButton_clicked();

    void on_pushButton_loadDatabase_clicked();

    void on_pushButton_update_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_pushButton_delete_clicked();

private:
    QString rowIndex;
    Ui::Users *ui;
  };

#endif // USERS_H
