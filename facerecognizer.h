#ifndef FACERECOGNIZER_H
#define FACERECOGNIZER_H

#include <QCoreApplication>
#include <opencv2/core/core.hpp>
#include <opencv2/face.hpp>
#include<opencv2/face/facerec.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include<list>


#include <iostream>
#include <fstream>
#include<string>
#include <sstream>
#include <math.h>
#include<time.h>

using namespace cv;
using namespace std;

class faceRecognizer
{
public:

    faceRecognizer();
    static void startRecognizer();
    static void startRecognizer(string path);
    static void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, char separator = ',');
};

#endif // FACERECOGNIZER_H
