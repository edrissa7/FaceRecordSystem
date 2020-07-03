#include "facedetect.h"



faceDetect::faceDetect(){
}
    void faceDetect::runDetector(){
string cascadeName, nestedCascadeName;



        // VideoCapture class for playing video for which faces to be detected
        VideoCapture capture;
        Mat frame, image;

     Rect myroi;//new region of interest cropper

        Mat crop; //for cropping

        // PreDefined trained XML classifiers with facial features
        CascadeClassifier cascade, nestedCascade;
        double scale=1;
         int count = 0;//image counter
        // Load classifiers from "opencv/data/haarcascades" directory

        // Change path before execution
        cascade.load( "C:/Users/user/Downloads/opencv/sources/data/haarcascades/haarcascade_frontalface_default.xml" ) ;


        capture.open(0);//am startinf the video using webcam(0)
        if( !capture.isOpened() )//checking whether the capture is opened
        {
            cout<<"Could not Open Camera";
              return;
          }
            // Capture frames from video and detect faces
            cout << "Face Detection Started...." << endl;
            while(1)
            {
                capture >> frame;
                if( frame.empty() )
                    break;
                Mat frame1 = frame.clone();
               this->detectAndDraw( frame1, cascade,scale,myroi);
                char c = waitKey(10);
                cvtColor(frame,image,COLOR_BGR2GRAY);
                // Press q to exit from window
                if(c == 'q'){
                    cout<<"face detection ended"<<endl;
                    cv::destroyAllWindows();
                    break;

                }else if(c == 't'){
                    QString s = QString::number(count);
                    std::string str = s.toUtf8().constData();//convert Qstring to regular string
                    crop = image(myroi);
                   file = QFileDialog::getSaveFileName();
                   if(!file.isEmpty()){ //checking whether the file is not empty
                   std::string filestr = file.toUtf8().constData();
                    cv::imwrite(filestr, crop);
                    count++;
                   }else{
                       QMessageBox::information();
                   }
                }

            }
    }



//detect and draw rectangle
    void faceDetect::detectAndDraw( Mat& img, CascadeClassifier& cascade,double scale,Rect& roi)
    {
        vector<Rect> faces;
        Mat gray, smallImg;

        cvtColor( img, gray, COLOR_BGR2GRAY ); // Convert to Gray Scale
        double fx = 1 / scale;

        // Resize the Grayscale Image
        resize( gray, smallImg, Size(), fx, fx, INTER_LINEAR );
        //It is a method that improves the contrast in an image, in order to stretch out the intensity range
        equalizeHist( smallImg, smallImg);

        //Detect faces of different sizes using cascade classifier
        cascade.detectMultiScale( smallImg, faces, 1.1,
                                2, 0|CASCADE_SCALE_IMAGE, Size(30, 30));
         foundface = true;
        //region of interest code
         if(foundface){//making sure there is a face before copying coordinates for cropping
         roi.x = faces[0].x;
         roi.y = faces[0].y;
         roi.width = faces[0].width;
         roi.height = faces[0].height;

        for ( size_t i = 0; i < faces.size(); i++ )
        {
            Rect r = faces[i];
            Scalar color = Scalar(255, 0, 0); // Color for Drawing rectangle

            //draw the rectangle on the face
            cv::rectangle(img,Point(r.x,r.y),Point((r.x+r.width),(r.y+r.height)),color,2);


   }
        imshow( "Face Detection", img );
}
    }


