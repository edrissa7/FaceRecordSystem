#include "videofacedetector.h"
//#include<dlib/image_processing/frontal_face_detector.h>
#include <dlib/opencv.h>
//#include <dlib/image_processing/render_face_detections.h>
//#include <dlib/image_processing.h>
//#include <dlib/gui_widgets.h>


const double VideoFaceDetector::TICK_FREQUENCY = cv::getTickFrequency();

 VideoFaceDetector::VideoFaceDetector(const std::string cascadeFilePath, cv::VideoCapture &videoCapture){
    setFaceCascade(cascadeFilePath);
    setVideoCapture(videoCapture);
}

//setup video capturing from webcam
cv::VideoCapture* VideoFaceDetector::setVideoCapture(cv::VideoCapture &videoCapture){
   m_videoCapture = &videoCapture;
   return m_videoCapture;
}

//Load cascade classfier xml file
cv::CascadeClassifier* VideoFaceDetector::setFaceCascade(const std::string cascadeFilePath){
    if (m_faceCascade == 0) {
            m_faceCascade = new cv::CascadeClassifier(cascadeFilePath);
        }
            else {
            m_faceCascade->load(cascadeFilePath);
        }

        if (m_faceCascade->empty()) {
            std::cerr << "Error creating cascade classifier. Make sure the file" << std::endl
                << cascadeFilePath << " exists." << std::endl;
        }

        return m_faceCascade;
    }
cv::Rect VideoFaceDetector::face()const{
    cv::Rect faceRect = m_trackedFace;
    faceRect.x = (int)(faceRect.x / m_scale);
    faceRect.y = (int)(faceRect.y / m_scale);
    faceRect.width = (int)(faceRect.width / m_scale);
    faceRect.height = (int)(faceRect.height / m_scale);
    return faceRect;

}

VideoFaceDetector::~VideoFaceDetector(){
    if(m_faceCascade != 0){
        delete m_faceCascade;
    }
}
cv::Point VideoFaceDetector::getFrameAndDetect(cv::Mat &frame)
{
    // Read in the frame
    *m_videoCapture >> frame;

    // Downscale frame to the specified width of "m_resizedWidth" while maintaing the aspect ratio
    // This is done for speed purposes
    m_scale = (double)min(m_resizedWidth, frame.cols) / frame.cols;
    cv::Size resizedFrameSize = cv::Size((int)(m_scale*frame.cols), (int)(m_scale*frame.rows));

    cv::Mat gray;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

    cv::Mat resizedFrame;
        resize(gray, resizedFrame, resizedFrameSize);

       // Check to see if we found a face in the previous frame
        if (!m_foundFace)
                detectFaceAllSizes(resizedFrame); // Detect using cascades over whole image
            else {
                detectFaceAroundRoi(resizedFrame); // Detect using cascades only in ROI
              if (m_templateMatchingRunning) {
               detectFacesTemplateMatching(resizedFrame); // Detect using template matching
              }
            }

    return m_facePosition;
}//-----------------------------------------------------------------------------------

// When the input operator " >> " is used call the function "getFrameAndDetect()"
// Then return the output of that function
cv::Point VideoFaceDetector::operator>>(cv::Mat &frame)
{
    return this->getFrameAndDetect(frame);
}

void VideoFaceDetector::detectFaceAroundRoi(const  cv::Mat &frame)
{
    /*frontal_face_detector detector = get_frontal_face_detector();
    cv_image<unsigned char> cimg(frame(m_faceRoi));
    std::vector<dlib::rectangle> faces = detector(cimg);

    for (int i = 0; i < faces.size(); i++){
        m_allFaces.push_back(dlibRectangleToOpenCV(faces[i]));
    }*/

    // Detect faces sized +/-20% off biggest face in previous search
    m_faceCascade->detectMultiScale(frame(m_faceRoi), m_allFaces, 1.1, 5, cv::CASCADE_DO_CANNY_PRUNING,
        cv::Size(m_trackedFace.width * 8 / 10, m_trackedFace.height * 8 / 10),
        cv::Size(m_trackedFace.width * 12 / 10, m_trackedFace.width * 12 / 10));

    if (m_allFaces.empty())
    {
        // Activate template matching if not already started and start timer
        m_templateMatchingRunning = true;
        if (m_templateMatchingStartTime == 0)
            m_templateMatchingStartTime = cv::getTickCount();
        return;
    }

    // Turn off template matching if running and reset timer
    m_templateMatchingRunning = false;
    m_templateMatchingCurrentTime = m_templateMatchingStartTime = 0;

    // Get detected face
    m_trackedFace = biggestFace(m_allFaces);

    // Add ROI offset to face so that the newly obtained face position
    // will be relative to the original full image frame rather than the smaller ROI
    m_trackedFace.x += m_faceRoi.x;
    m_trackedFace.y += m_faceRoi.y;

    // Get face template
    m_faceTemplate = getFaceTemplate(frame, m_trackedFace);

    // Calculate ROI
    m_faceRoi = doubleRectSize(m_trackedFace, cv::Rect(0, 0, frame.cols, frame.rows));

    // Update face position
    m_facePosition = centerOfRect(m_trackedFace);
}


cv::Rect VideoFaceDetector::doubleRectSize(const  cv::Rect &inputRect, const  cv::Rect &frameSize) const
{
    cv::Rect outputRect;
    // Double rect size
    outputRect.width = inputRect.width * 2;
    outputRect.height = inputRect.height * 2;

    // Center rect around original center
    outputRect.x = inputRect.x - inputRect.width / 2;
    outputRect.y = inputRect.y - inputRect.height / 2;

    // Handle edge case by stopping the ROI right up againts the edge of the image frame
    // Line up the edge of the ROI with the edge of the image frame
    // Update the position of the ROI rectangle according to the above maximum
    if (outputRect.x < frameSize.x) {
        outputRect.width += outputRect.x;
        outputRect.x = frameSize.x;
    }
    if (outputRect.y < frameSize.y) {
        outputRect.height += outputRect.y;
        outputRect.y = frameSize.y;
    }

    if (outputRect.x + outputRect.width > frameSize.width) {
        outputRect.width = frameSize.width - outputRect.x;
    }
    if (outputRect.y + outputRect.height > frameSize.height) {
        outputRect.height = frameSize.height - outputRect.y;
    }

    return outputRect;
}
// Find the biggest face that was detected
cv::Rect VideoFaceDetector::biggestFace(std::vector<cv::Rect> &faces) const
{
    assert(!faces.empty()); // Give an error if this function ran without detected faces

    cv::Rect *biggest = &faces[0];
    for (int i = 0; i < faces.size(); i++) {
        cv::Rect &face = faces[i];
        if (face.area() < biggest->area())
            biggest = &face;
    }
    return *biggest;
}

// Gets a face template to be used for fast and accurate tracking in the next frame
// The face template is simply a small region in the middle of the face. This is the
// most unique from person to person and will also change the least from frame to frame
cv::Mat VideoFaceDetector::getFaceTemplate(const  cv::Mat &frame, cv::Rect face)
{
    face.x += face.width / 4;
    face.y += face.height / 4;
    face.width /= 2;
    face.height /= 2;

    cv::Mat faceTemplate = frame(face).clone();
    return faceTemplate;
}
//--------------------------------------------------------------

cv::Point VideoFaceDetector::centerOfRect(const  cv::Rect &rect) const
{
    return  cv::Point(rect.x + rect.width / 2, rect.y + rect.height / 2);
}


void VideoFaceDetector::detectFacesTemplateMatching(const  cv::Mat &frame)
{
    // Calculate duration of template matching
    m_templateMatchingCurrentTime = cv::getTickCount();
    double duration = (double)(m_templateMatchingCurrentTime - m_templateMatchingStartTime) / TICK_FREQUENCY;

    // If template matching lasts for more than 2 seconds face is possibly lost
    // so disable it and redetect using cascades
    if (duration > m_templateMatchingMaxDuration) {
        m_foundFace = false;
        m_templateMatchingRunning = false;
        m_templateMatchingStartTime = m_templateMatchingCurrentTime = 0;
    }

    // Template matching with last known face, searching within the face ROI
    // matchTemplate(frame(m_faceRoi), m_faceTemplate, m_matchingResult, CV_TM_CCOEFF);
    matchTemplate(frame(m_faceRoi), m_faceTemplate, m_matchingResult, cv::TM_SQDIFF);
    normalize(m_matchingResult, m_matchingResult, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());
    double min, max;
    cv::Point minLoc, maxLoc;
    minMaxLoc(m_matchingResult, &min, &max, &minLoc, &maxLoc);

    // Add roi offset to face position
    minLoc.x += m_faceRoi.x;
    minLoc.y += m_faceRoi.y;

    // Get detected face
    //m_trackedFace =  Rect(maxLoc.x, maxLoc.y, m_trackedFace.width, m_trackedFace.height);
    m_trackedFace = cv::Rect(minLoc.x, minLoc.y, m_faceTemplate.cols, m_faceTemplate.rows);
    m_trackedFace = doubleRectSize(m_trackedFace, cv::Rect(0, 0, frame.cols, frame.rows));

    // Get new face template
    m_faceTemplate = getFaceTemplate(frame, m_trackedFace);

    // Calculate face ROI
    m_faceRoi = doubleRectSize(m_trackedFace, cv::Rect(0, 0, frame.cols, frame.rows));

    // Update face position
    m_facePosition = centerOfRect(m_trackedFace);
}
//----------------------------------------------------------------------------------

// Since we have not detected a face in the ROI, i.e "m_foundFace == false"
// Run the face detection on the entire image frame
void VideoFaceDetector::detectFaceAllSizes(const  cv::Mat &frame)
{

    // Minimum face size is 1/5th of screen height
    // Maximum face size is 2/3rds of screen height
    m_faceCascade->detectMultiScale(frame, m_allFaces, 1.1, 5, cv::CASCADE_DO_CANNY_PRUNING,
        cv::Size(frame.rows / 5, frame.rows / 5),
        cv::Size(frame.rows * 2 / 3, frame.rows * 2 / 3));

    // If no faces were found, exit the function and move to the next method
    // of detecting a face (see the "getFrameAndDetect()" function)
    if (m_allFaces.empty()) { return; }

    // We have found a face
    m_foundFace = true;

    // Locate biggest face as this is most likely to be correct
    m_trackedFace = biggestFace(m_allFaces);

    // Get a face template to be used for the next frame detection
    m_faceTemplate = getFaceTemplate(frame, m_trackedFace);

    // Calculate ROI to be used for the next frame detection
    m_faceRoi = doubleRectSize(m_trackedFace, cv::Rect(0, 0, frame.cols, frame.rows));

    // Update face position
    m_facePosition = centerOfRect(m_trackedFace);
}
//-----------------------------------------------------------------------------

double VideoFaceDetector::setTemplateMatchingMaxDuration(const double s)
{
    m_templateMatchingMaxDuration = s;
    return m_templateMatchingMaxDuration;
}
cv::Point VideoFaceDetector::facePosition() const
{
    cv::Point facePos;
    facePos.x = (int)(m_facePosition.x / m_scale);
    facePos.y = (int)(m_facePosition.y / m_scale);
    return facePos;
}



