#include "facerecognizer.h"
//#include "imagesave.h"
#include "menu.h"

//to read from ext file and add pictures to images vector
void faceRecognizer::read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, char separator) {
    std::ifstream file(filename.c_str(), ifstream::in);
    if (!file) {
        string error_message = "No valid input file was given, please check the given filename.";
        //CV_Error(cv::AStsBadArg, error_message);
    }
    string line, path, classlabel;
    while (getline(file, line)) {
        stringstream liness(line);
        getline(liness, path, separator);
        getline(liness, classlabel);
        if (!path.empty() && !classlabel.empty()) {
            images.push_back(imread(path, 0));
            labels.push_back(atoi(classlabel.c_str()));
        }
    }
}
faceRecognizer::faceRecognizer()
{

}

void faceRecognizer::startRecognizer() {
  string listname[1000];
   // {
       // "edrissa"
        //"taira",
        //"AbdouK",
       // "CHris"
  //  };


 // vector<string> listname;
  //listname[] = "edrissa";
// list<string> listname;
 // listname.push_back("edrissa");

  //--------------------------------
  vector<string> Names1;
  vector<int> labels1;

  string thefile = "C:/Users/user/Desktop/Names.ext";
  std::ifstream file(thefile.c_str(), ifstream::in);
  if (!file) {
      string error_message = "No valid input file was given, please check the given filename.";
      //CV_Error(cv::AStsBadArg, error_message);
  }
  string line, name, classlabel;
  while (getline(file, line)) {
      stringstream liness(line);
      getline(liness, classlabel,',');
      getline(liness, name);
      if (!classlabel.empty() && !name.empty()) {
          Names1.push_back(name);
          labels1.push_back(atoi(classlabel.c_str()));
  }
  }
  //----------------------------------

  for(int i = 0; i < labels1.size(); i++){
  listname[i] = Names1.at(i);
  }



    string fn_csv = "C:/Users/user/Desktop/LBtest.ext";//file to load training set
   // int deviceId = 0;			// here is my webcam Id.
    //int detect_check = -1;
    int label = 1;

    // These vectors hold the images and corresponding labels:
    vector<Mat> images;
    vector<int> labels;

    // Read in the data (fails if no valid input filename is given, but you'll get an error message):
    try {
        read_csv(fn_csv, images, labels);
    }
    catch (cv::Exception& e) {
        cerr << "Error opening file \"" << fn_csv << "\". Reason: " << e.msg << endl;
        // nothing more we can do
        exit(1);
    }

    int img_width = images[0].cols;
    int img_height = images[0].rows;

    cout << "start recognizing..." << endl;

    string classifier = "C:/Users/user/Downloads/opencv/sources/data/haarcascades/haarcascade_frontalface_default.xml";
    CascadeClassifier face_cascade;
    face_cascade.load(classifier);

    Ptr<face::FaceRecognizer> model = face::LBPHFaceRecognizer::create(5);
    model->read("C:/Users/user/Desktop/LBPHface.yml");

    string window = "face detection";

    VideoCapture cap(0);

    if (!cap.isOpened())
    {
        cout << "exit" << endl;
        exit(1);

    }

    namedWindow(window, 1);
    long count = 0;

    while (true)
    {
        vector<Rect> faces;
        Mat frame;
        Mat graySacleFrame;
        Mat original;

        cap >> frame;

        count = count + 1;//count frames;

        if (!frame.empty()){

            //clone from original frame
            original = frame.clone();

            //convert image to gray scale and equalize
            cvtColor(original, graySacleFrame, COLOR_BGR2GRAY);

            //detect face in gray image
            face_cascade.detectMultiScale(graySacleFrame, faces, 1.1, 3, 0, cv::Size(90, 90));

            //number of faces detected
            std::string frameset = std::to_string(count);
            std::string faceset = std::to_string(faces.size());

           // int width = 0, height = 0;

            //person name
            string Pname = "";

            for (int i = 0; i < faces.size(); i++)
            {
                //region of interest
                Rect face_i = faces[i];

                //crop the roi from grya image
                Mat face = graySacleFrame(face_i);

                //resizing the cropped image to suit to database image sizes
                Mat face_resized;
                cv::resize(face, face_resized, Size(img_width, img_height), 1.0, 1.0, INTER_CUBIC);

                //recognizing what faces detected
                label = model->predict(face_resized);

                //drawing green rectagle in recognize face
                rectangle(original, face_i, CV_RGB(0, 255, 0), 1);
                string text = "Detected";
                if (label >= 0/* && label <= 2*/)
                {
                    Pname = listname[label];
                }
                else
                {
                    Pname = "unknown";
                    cout << "unknown person";
                }


                int pos_x = max(face_i.tl().x - 10, 0);
                int pos_y = max(face_i.tl().y - 10, 0);

                //name the person who is in the image
                putText(original, text, Point(pos_x, pos_y), FONT_HERSHEY_COMPLEX_SMALL, 1.0, CV_RGB(0, 255, 0), 1.0);

            }


            putText(original, "Frames: " + frameset, Point(30, 60), FONT_HERSHEY_COMPLEX_SMALL, 1.0, CV_RGB(0, 255, 0), 1.0);
            putText(original, "Person: " + Pname, Point(30, 90), FONT_HERSHEY_COMPLEX_SMALL, 1.0, CV_RGB(0, 255, 0), 1.0);
            //display to the winodw
            cv::imshow(window, original);

            if (waitKey(10) == 'q' /*&& label>=0*/)
            {
                label = -1;
                destroyAllWindows();
                break;
               // return Pname;
            }

        }
    }
}

//-------------------------------------------------------------------------------
void faceRecognizer::startRecognizer(string path) {
  string listname[1000];
   // {
       // "edrissa"
        //"taira",
        //"AbdouK",
       // "CHris"
  //  };


 // vector<string> listname;
  //listname[] = "edrissa";
// list<string> listname;
 // listname.push_back("edrissa");

  //--------------------------------
  vector<string> Names1;
  vector<int> labels1;

  string thefile = "C:/Users/user/Desktop/Names.ext";
  std::ifstream file(thefile.c_str(), ifstream::in);
  if (!file) {
      string error_message = "No valid input file was given, please check the given filename.";
      //CV_Error(cv::AStsBadArg, error_message);
  }
  string line, name, classlabel;
  while (getline(file, line)) {
      stringstream liness(line);
      getline(liness, classlabel,',');
      getline(liness, name);
      if (!classlabel.empty() && !name.empty()) {
          Names1.push_back(name);
          labels1.push_back(atoi(classlabel.c_str()));
  }
  }
  //----------------------------------

  for(int i = 0; i < labels1.size(); i++){
  listname[i] = Names1.at(i);
  }



    string fn_csv = "C:/Users/user/Desktop/LBtest.ext";//file to load training set
   // int deviceId = 0;			// here is my webcam Id.
    //int detect_check = -1;
    int label = 1;

    // These vectors hold the images and corresponding labels:
    vector<Mat> images;
    vector<int> labels;

    // Read in the data (fails if no valid input filename is given, but you'll get an error message):
    try {
        read_csv(fn_csv, images, labels);
    }
    catch (cv::Exception& e) {
        cerr << "Error opening file \"" << fn_csv << "\". Reason: " << e.msg << endl;
        // nothing more we can do
        exit(1);
    }

    int img_width = images[0].cols;
    int img_height = images[0].rows;

    cout << "start recognizing..." << endl;

    string classifier = "C:/Users/user/Downloads/opencv/sources/data/haarcascades/haarcascade_frontalface_default.xml";
    CascadeClassifier face_cascade;
    face_cascade.load(classifier);

    Ptr<face::FaceRecognizer> model = face::LBPHFaceRecognizer::create(5);
    model->read("C:/Users/user/Desktop/LBPHface.yml");

    string window = "face detection";

    VideoCapture cap(path);

    if (!cap.isOpened())
    {
        cout << "exit" << endl;
        exit(1);

    }

    namedWindow(window, 1);
    long count = 0;

    while (true)
    {
        vector<Rect> faces;
        Mat frame;
        Mat graySacleFrame;
        Mat original;

        cap >> frame;

        count = count + 1;//count frames;

        if (!frame.empty()){

            //clone from original frame
            original = frame.clone();

            //convert image to gray scale and equalize
            cvtColor(original, graySacleFrame, COLOR_BGR2GRAY);

            //detect face in gray image
            face_cascade.detectMultiScale(graySacleFrame, faces, 1.1, 3, 0, cv::Size(90, 90));

            //number of faces detected
            std::string frameset = std::to_string(count);
            std::string faceset = std::to_string(faces.size());

           // int width = 0, height = 0;

            //person name
            string Pname = "";

            for (int i = 0; i < faces.size(); i++)
            {
                //region of interest
                Rect face_i = faces[i];

                //crop the roi from grya image
                Mat face = graySacleFrame(face_i);

                //resizing the cropped image to suit to database image sizes
                Mat face_resized;
                cv::resize(face, face_resized, Size(img_width, img_height), 1.0, 1.0, INTER_CUBIC);

                //recognizing what faces detected
                label = model->predict(face_resized);

                //drawing green rectagle in recognize face
                rectangle(original, face_i, CV_RGB(0, 255, 0), 1);
                string text = "Detected";
                if (label >= 0/* && label <= 2*/)
                {
                    Pname = listname[label];
                }
                else
                {
                    Pname = "unknown";
                    cout << "unknown person";
                }


                int pos_x = max(face_i.tl().x - 10, 0);
                int pos_y = max(face_i.tl().y - 10, 0);

                //name the person who is in the image
                putText(original, text, Point(pos_x, pos_y), FONT_HERSHEY_COMPLEX_SMALL, 1.0, CV_RGB(0, 255, 0), 1.0);

            }


            putText(original, "Frames: " + frameset, Point(30, 60), FONT_HERSHEY_COMPLEX_SMALL, 1.0, CV_RGB(0, 255, 0), 1.0);
            putText(original, "Person: " + Pname, Point(30, 90), FONT_HERSHEY_COMPLEX_SMALL, 1.0, CV_RGB(0, 255, 0), 1.0);
            //display to the winodw
            cv::imshow(window, original);

            if (waitKey(50) == 'q' /*&& label>=0*/)
            {
                label = -1;
                destroyAllWindows();
                break;
               // return Pname;
            }

        }
    }
}

