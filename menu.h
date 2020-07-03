#ifndef MENU_H
#define MENU_H

#include <QMainWindow>
#include<QtSql>
#include<QtDebug>
#include<QFileInfo>
#include<QDate>

#include<QFileDialog>
#include<QString>
#include<QMessageBox>
#include<QTextStream>
#include<QFile>

#include<string>
#include<iostream>

#include<opencv2/core.hpp>
#include<opencv2/highgui.hpp> //for GUI creation
#include<opencv2/imgcodecs.hpp>

#include<opencv2/opencv.hpp>
#include<opencv2/objdetect.hpp>
#include<opencv2/features2d.hpp>


#include "facerecognizer.h"
#include "imagesave.h"
#include "datasettrainer.h"



namespace Ui {
class Menu;
}

class Menu : public QMainWindow
{
    Q_OBJECT

public:
    string returnName;
    QString rowIndex;
    explicit Menu(QWidget *parent = nullptr);
    //dectector public members-------------------------------
    QString file;
    Mat crop; //for cropping
    QString imageurl;
    QString trainer = "C:/Users/user/Desktop/LBtest.ext";//file for training data
     Rect regionOI(vector<Rect>& faceroi);//region of interest to crop
     // Function for Face Detection
     void detectAndDraw( Mat& img, CascadeClassifier& cascade,double scale,Rect& roi);
     void runDetector();
     //--------------------------------------------------------------------
     //trainer------------
     dataSetTrainer dataTrainer;
     //------------------
     imageSave window;
     ~Menu();


private slots:
     void on_button_StartDetect_clicked();
     void on_button_AddUser_clicked();

     void UpdateAge(QDate date);

     void on_checkBox_stateChanged(int arg1);

     void on_checkBox_editData_clicked();

     void on_pushButton_loadData_clicked();

     void on_pushButton_dataSave_clicked();

     void on_pushButton_train_clicked();

     void on_pushButton_add_clicked();

     void on_pushButton_searchPerson_clicked();

     void on_tableView_clicked(const QModelIndex &index);

     void on_pushButton_2_clicked();

     void on_pushButton_3_clicked();

signals:
    void dateChanged(QDate newdate);
private:
    //detector private members----------------
     bool foundface = false;
     //----------------------------------------
    //database handler
    QSqlDatabase myDb;
     QString filename = "C:/Users/user/Desktop/LBtest.ext";

    //so other connect method can see filename
    QString url;
    Ui::Menu *ui;



};

#endif // MENU_H
