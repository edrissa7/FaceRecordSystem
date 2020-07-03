#include "imagesave.h"
#include "ui_imagesave.h"
#include "login.h"

imageSave::imageSave(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::imageSave)
{
    ui->setupUi(this);    
    ui->label_image->setPixmap(myfile);//setting the image to the Mat.
}

imageSave::~imageSave()
{
    delete ui;
}

void imageSave::on_pushButton_loadImage_clicked()
{
    myfile = QFileDialog::getSaveFileName();
    if(!myfile.isEmpty()){
        ui->lineEdit_imagePath->setText(myfile);
    }else{
        QMessageBox::critical(this,"message","image not saved");
    }
}

void imageSave::on_pushButton_clicked()
{
    classlabel = ui->spinBox_classlabel->text();
    personName = ui->lineEdit_name->text();


    QString trainMaker = myfile+","+classlabel+"\n";
    std::string data = trainMaker.toUtf8().constData();


//checking whether the file is not empty
    if(!myfile.isEmpty() && !personName.isEmpty() && !classlabel.isEmpty()){
        //--------------------------------------------------
        QString itNames = classlabel+","+personName+"\n";
        std::string namesData = itNames.toUtf8().constData();

        outputNames.open("C:/Users/user/Desktop/Names.ext",std::ios_base::app);
        outputNames << namesData;
        outputNames.close();
        //----------------------------------------------



    std::string filestr =  myfile.toUtf8().constData();
    cv::imwrite(filestr,myCrop);
    QMessageBox::information(this,"message","image saved successfully");

    outputfile.open("C:/Users/user/Desktop/LBtest.ext",std::ios_base::app);
    outputfile << data;
    outputfile.close();
    this->close();
    }else{
         QMessageBox::critical(this,"message","fill all fields");
    }
}

void imageSave::on_pushButton_cancelSave_clicked()
{
    this->close();
}

/*void imageSave::on_pushButton_2_clicked()
{
    Login conn;
    QString path = ui->lineEdit_imagePath->text();
    int label = ui->spinBox_classlabel->value();
    int rid = ui->spinBox_personId->value();

    QString insertQuery = "INSERT INTO pathRecords(path,label,rid)"
                          "VALUES(:path,:label,:rid)";

   if(!conn.connOpen()){
       qDebug()<<("Failed to open connection");
       return;
   }
   conn.connOpen();
    QSqlQuery query;
    query.prepare(insertQuery);
    query.bindValue(":path",path);
      query.bindValue(":label",label);
       query.bindValue(":rid",rid);


    if(!query.exec()){
        QMessageBox::critical(this,"error",query.lastError().text());

    }else{
       QMessageBox::information(this,"Message","Data Inserted");
       conn.connClose();
    }

   }*/




