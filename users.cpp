#include "users.h"
#include "ui_users.h"
#include<iostream>


#include "login.h"

Users::Users(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Users)
{

    ui->setupUi(this);
     loadTable();
    /*Login conn;
    if(!conn.connOpen()){
        ui->label_connStatus->setText("Database connection failed!");
    }else{
        ui->label_connStatus->setText("Database Connected");
    }*/
}

Users::~Users()
{
    delete ui;
}

void Users::on_pushButton_clicked()
{
    Login conn;
    QDate mydate;
    QString date = mydate.currentDate().toString();


    QString firstname,lastname,username,password,usertype;
     firstname = ui->lineEdit_firstname->text();
     lastname = ui->lineEdit_lastname->text();
     username = ui->lineEdit_username->text();
     password = ui->lineEdit_password->text();
     usertype = ui->comboBox->currentText();

     QString insertQuery = "INSERT INTO users(id,firstname,lastname,username,password,dateAdded,user_type)"
                           "VALUES(:id,:firstname,:lastname,:username,:password,:dateAdded,:usertype)";

    if(!conn.connOpen()){
        qDebug()<<("Failed to open connection");
        return;
    }
    conn.connOpen();
     QSqlQuery query;
     query.prepare(insertQuery);
     query.bindValue(":id",ui->spinBox_rowid->value());
     query.bindValue(":firstname",firstname);
       query.bindValue(":lastname",lastname);
         query.bindValue(":username",username);
           query.bindValue(":password",password);
           query.bindValue(":dateAdded",date);
             query.bindValue(":usertype",usertype);

     if(!query.exec()){
         QMessageBox::critical(this,"error",query.lastError().text());

     }else{
        QMessageBox::information(this,"Message","Data Inserted");
        ui->lineEdit_firstname->setText("");
         ui->lineEdit_lastname->setText("");
         ui->lineEdit_username->setText("");
         ui->lineEdit_password->setText("");
         ui->comboBox->setEditText("");
       ui->spinBox_rowid->setValue(0);
        conn.connClose();
         loadTable();
     }

    }



void Users::on_pushButton_loadDatabase_clicked()
{
Login conn;
QSqlQueryModel *model = new QSqlQueryModel();
conn.connOpen();
QSqlQuery *query= new QSqlQuery(conn.mydb);
query->prepare("select * from users");
query->exec();
model->setQuery(*query);

ui->tableView->setModel(model);
conn.connClose();
}

void Users::on_pushButton_update_clicked()
{
    Login conn;
    QDate mydate;
    QString date = mydate.currentDate().toString();


    QString firstname,lastname,username,password,usertype,rowid;
     firstname = ui->lineEdit_firstname->text();
     lastname = ui->lineEdit_lastname->text();
     username = ui->lineEdit_username->text();
     password = ui->lineEdit_password->text();
     usertype = ui->comboBox->currentText();
     rowid = ui->spinBox_rowid->text();

    /* QString insertQuery = "update users set firstname=:firstname,lastname = :lastname,"
                           "username=:username,password=:password,dateAdded= :dateAdded,user_type=:usertype"
                           "where rowid = '"+rowIndex+"'";*/

   QString insertQuery = "UPDATE users SET firstname=?,lastname = ?,"
                         "username=?,password=?,user_type=?"
                         " where id = ?";

    if(!conn.connOpen()){
        qDebug()<<("Failed to open connection");
        return;
    }
    conn.connOpen();
     QSqlQuery query;
     query.prepare(insertQuery);
     query.addBindValue(firstname);
     query.addBindValue(lastname);
     query.addBindValue(username);
     query.addBindValue(password);
     query.addBindValue(usertype);
     query.addBindValue(ui->spinBox_rowid->value());
    /* query.prepare(insertQuery);
     query.bindValue(":firstname",firstname);
       query.bindValue(":lastname",lastname);
         query.bindValue(":username",username);
           query.bindValue(":password",password);
           query.bindValue(":dateAdded",date);
             query.bindValue(":usertype",usertype);*/


     if(!query.exec()){
         QMessageBox::critical(this,"error",query.lastError().text());

     }else{
        QMessageBox::information(this,"Message","Row updated");
        ui->lineEdit_firstname->setText("");
         ui->lineEdit_lastname->setText("");
         ui->lineEdit_username->setText("");
         ui->lineEdit_password->setText("");
         ui->comboBox->setEditText("");
       ui->spinBox_rowid->setValue(0);
        conn.connClose();
         loadTable();
     }
}

void Users::on_tableView_clicked(const QModelIndex &index)
{

    rowIndex = QString::number(index.row()+1);

   Login conn;
    if(!conn.connOpen()){
        qDebug()<<"failed to open database";
        return;
    }

  cout << rowIndex.toUtf8().constData()<<endl;

    conn.connOpen();
    QSqlQuery qry;
    //qry.prepare("select firstname,lastname,username,password,user_type,rowid from users where rowid=6");
    qry.prepare("select * from users where rowid='"+rowIndex+"'");
    if(qry.exec()){
       int count = 0;
        while(qry.next()){
            count++;
            ui->lineEdit_firstname->setText(qry.value(1).toString());
             ui->lineEdit_lastname->setText(qry.value(2).toString());
             ui->lineEdit_username->setText(qry.value(3).toString());
             ui->lineEdit_password->setText(qry.value(4).toString());
             ui->comboBox->setEditText(qry.value(6).toString());
           ui->spinBox_rowid->setValue((qry.value(0).toInt()));
   // cout << qry.value(6).toString().toUtf8().constData()<<endl;
        }
        if(count == 1){
         //ui->label->setText("Username and password are correct");
         conn.connClose();
    // QString firstname,lastname,username,password,usertype,rowid;
}
}
}

void Users::on_pushButton_delete_clicked()
{
    Login conn;
    QString id = ui->spinBox_rowid->text();


    if(!conn.connOpen()){
        qDebug()<<"failed to open database";
        return;
    }

    conn.connOpen();
    QSqlQuery qry;
    qry.prepare("delete from users where id='"+id+"'");
    if(qry.exec()){
       QMessageBox::information(this,"Success!","Delete Successful");
       ui->lineEdit_firstname->setText("");
        ui->lineEdit_lastname->setText("");
        ui->lineEdit_username->setText("");
        ui->lineEdit_password->setText("");
        ui->comboBox->setEditText("");
      ui->spinBox_rowid->setValue(0);
       conn.connClose();
       loadTable();
      }
}

void Users::loadTable(){
    Login conn;
    QSqlQueryModel *model = new QSqlQueryModel();
    conn.connOpen();
    QSqlQuery *query= new QSqlQuery(conn.mydb);
    query->prepare("select * from users");
    query->exec();
    model->setQuery(*query);

    ui->tableView->setModel(model);
    conn.connClose();
}
