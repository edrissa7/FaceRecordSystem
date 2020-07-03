#ifndef VIDEOFACEDETECTOR_H
#define VIDEOFACEDETECTOR_H

#include<opencv2/core.hpp>
#include<opencv2/highgui.hpp> //for GUI creation
#include<opencv2/imgcodecs.hpp>
#include<opencv2/opencv.hpp>

using namespace std;

class VideoFaceDetector{
public:
    //run the detector
    VideoFaceDetector(const std::string cascadeFilePath, cv::VideoCapture &videoCapture );
    ~VideoFaceDetector();
    //setup and return webcam;
    cv::VideoCapture *setVideoCapture(cv::VideoCapture &videoCapture);
    //xml face classfier file setup
    cv::CascadeClassifier *setFaceCascade(const std::string cascadeFilePath);
    //outlines the face in the video frame with a rectangle
    cv::Rect face()const;
    // Set the center of the face rectangle
    cv::Point facePosition() const;
    cv::Point operator>>(cv::Mat &frame);
    // Reads in an image frame, resizes it, and calls the appropriate detection function
    cv::Point  getFrameAndDetect(cv::Mat &frame);
    //Sets the variable for the resized frame width
     int setResizedWidth(const int width);
     // Sets a timer for how long to try template matching
     double setTemplateMatchingMaxDuration(const double s);
private:
    //detector stuff
     static const double TICK_FREQUENCY; // Created here so it can be accessed by many functions

    cv::CascadeClassifier *m_faceCascade = 0; //cascase class null pointer
    cv::VideoCapture *m_videoCapture = 0;//videocapture class null pointer
    cv::Rect m_trackedFace; //The chosen face to be tracked (currently set as the largest found face
    double m_scale;//Aspect ratio to be maintained
    cv::Mat m_faceTemplate; // A rectangular of pixels as a face template that is a small region in the middle of the face
    int m_resizedWidth = 400; // Downsize the input frame for speed
    cv::Point m_facePosition; // The center of the face rectangle
    bool m_foundFace = false; // Check to see if we found a face in the previous frame
    cv::Rect m_faceRoi; // A smaller region of interest around the previously detected face if one was detected in the last frame
    std::vector<cv::Rect> m_allFaces; // All faces that were found in the "detectMultiscale" search
    bool m_templateMatchingRunning = false; // Check if we are running the template matching
    int64 m_templateMatchingStartTime = 0; //These two are used to measure how long template matching is running
    int64 m_templateMatchingCurrentTime = 0; //and to stop it after a set duration specified by "m_templateMatchingMaxDuration"
    double m_templateMatchingMaxDuration = 1; // Sets a timer for how long to run template matching
    cv::Mat  m_matchingResult; // The resulting rectangle of pixels (best match) from template matching


    void detectFacesTemplateMatching(const cv::Mat &frame); // Detect faces using template matching where the template is from a face of the previous frame

    cv::Point centerOfRect(const cv::Rect &rect) const; // The center point of the detected face box
    cv::Rect doubleRectSize(const cv::Rect &inputRect, const cv::Rect &frameSize) const; // The ROI is double the previously detected face size
    cv::Mat getFaceTemplate(const cv::Mat &frame, cv::Rect face); // Create a face template to be used for template matching
    void detectFaceAllSizes(const cv::Mat &frame); // Detect faces using cascades with "detectMultiscale" in the whole image
    void detectFaceAroundRoi(const cv::Mat &frame); // Detect faces using cascades with "detectMultiscale" in the ROI
    cv::Rect biggestFace(std::vector<cv::Rect> &faces) const; // From all the detected faces, grab the biggest (usually the most likely) one
};

#endif // VIDEOFACEDETECTOR_H
