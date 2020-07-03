#ifndef IMAGESAVE_H
#define IMAGESAVE_H

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>
#include<string>


#include<opencv2/core.hpp>
#include<opencv2/highgui.hpp> //for GUI creation
#include<opencv2/imgcodecs.hpp>
#include<opencv2/opencv.hpp>

#include<fstream>



namespace Ui {
class imageSave;
}

class imageSave : public QDialog
{
    Q_OBJECT

public:
    explicit imageSave(QWidget *parent = nullptr);
    ~imageSave();
    QString myfile;
    QString classlabel;
    QString personName;
    cv::Mat myCrop;
    QLabel imageLabel;


private slots:
    void on_pushButton_loadImage_clicked();

    void on_pushButton_clicked();

    void on_pushButton_cancelSave_clicked();

  //  void on_pushButton_2_clicked();

private:
    Ui::imageSave *ui;
    std::ofstream outputfile;
    std::ofstream outputNames;
};

#endif // IMAGESAVE_H
