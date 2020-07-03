#ifndef DATASETTRAINER_H
#define DATASETTRAINER_H

#include <QCoreApplication>
#include <opencv2/core/core.hpp>
#include <opencv2/face.hpp>
#include<opencv2/face/facerec.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include<time.h>

#include<QString>

using namespace cv;
using namespace std;

class dataSetTrainer
{
private:
     Ptr<face::FaceRecognizer> model;
public:
    dataSetTrainer();
    void runTrainer();
    //loads the images and their corresponding labels(seperated by ',') from the ext file
     static void dbread(const string& filename, vector<Mat>& images, vector<int>& labels, char separator = ',');
};

#endif // DATASETTRAINER_H
