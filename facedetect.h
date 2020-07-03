#ifndef FACEDETECT_H
#define FACEDETECT_H

#include <QCoreApplication>
#include"opencv2/highgui.hpp"
#include"opencv2/imgproc.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/core.hpp"
#include"opencv2/face/facerec.hpp"

#include<iostream>
#include<QFileDialog>
#include <QDialog>
#include<QMessageBox>

using namespace cv;
using namespace std;

class faceDetect
{
private:

     bool foundface = false;

public:
     QString file;
     QString imageurl;
     QString trainer = "C:/Users/user/Desktop/testfile.ext";
    void runDetector();
    static Rect regionOI(vector<Rect>& faceroi);//region of interest to crop
    // Function for Face Detection
    void detectAndDraw( Mat& img, CascadeClassifier& cascade,double scale,Rect& roi);
    faceDetect();

};

#endif // FACEDETECT_H
