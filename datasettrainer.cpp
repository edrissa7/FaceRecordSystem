#include "datasettrainer.h"
#include <opencv2/face.hpp>
#include<opencv2/face/facerec.hpp>


dataSetTrainer::dataSetTrainer()
{


}
 void dataSetTrainer::dbread(const string& filename, vector<Mat>& images, vector<int>& labels, char separator){
    std::ifstream file(filename.c_str(), ifstream::in);

    if (!file){
        string error = "no valid input file";
       // CV_Error(CV_StsBadArg, error);
    }

    string line, path, label;
    while (getline(file, line))
    {
        stringstream liness(line);
        getline(liness, path, separator);
        getline(liness, label);
        if (!path.empty() && !label.empty()){
            images.push_back(imread(path, 0));
            labels.push_back(atoi(label.c_str()));
        }
    }
}

void dataSetTrainer::runTrainer(){
    vector<Mat> images;
    vector<int> labels;

    try{
        string filename = "C:/Users/user/Desktop/LBtest.ext";
        dbread(filename, images, labels);

        cout << "size of the images is " << images.size() << endl;
        cout << "size of the labes is " << labels.size() << endl;
        cout << "Training begins...." << endl;
    }
    catch (cv::Exception& e){
        cerr << " Error opening the file " << e.msg << endl;
        exit(1);
    }

    //lbph face recognier model
    Ptr<face::FaceRecognizer> model = face::LBPHFaceRecognizer::create();

    //training images with relevant labels
    model->train(images, labels);

    //save the data in yaml file
    model->save("C:/Users/user/Desktop/LBPHface.yml");

    cout << "training finished...." << endl;
}
