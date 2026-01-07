#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <chrono>
#include <ctime>
#include <iomanip>

std::string haarCascadePath = "./haarcascade_frontalface_default.xml";

int main(){

    cv::CascadeClassifier faceCascade;
    if (!faceCascade.load(haarCascadePath)) {return 1;}

    cv::VideoCapture capture(0);

    if (!capture.isOpened()) {return 1;}

    std::cout << "Camera is now open. Press 'q' to exit." << std::endl;

    bool privacyToggle = true;

    while (true) {
        cv::Mat frame;
        capture >> frame;

        cv::putText(frame, "Scanning...", cv::Point((frame.cols / 2.5), 30), cv::FONT_HERSHEY_SIMPLEX, 0.9, cv::Scalar(0, 255, 0), 2);

        cv::Mat grayFrame;
        cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);

        cv::equalizeHist(grayFrame, grayFrame);

        std::vector<cv::Rect> faces;
        faceCascade.detectMultiScale(grayFrame, faces, 1.2, 9, 0, cv::Size(30, 30));

        
        for (int i = 0; i < faces.size(); i++) {

            if (privacyToggle) {
                cv::GaussianBlur(frame(faces[i]), frame(faces[i]), cv::Size(21, 21), 7);
            }

            double faceArea = faces[i].area();
            std::string textOnFace = "Face";
            cv::Scalar displayColor(0, 255, 0);

            if (faceArea > 45000) {
                textOnFace = "WARNING: PROXIMITY ALERT";
                displayColor = cv::Scalar(0, 0, 255);
            }

            cv::rectangle(frame, faces[i], displayColor, 2);
            cv::putText(frame, textOnFace, cv::Point(faces[i].x + (faces[i].width/3), faces[i].y - 20), cv::FONT_HERSHEY_SIMPLEX, 0.5, displayColor, 2);

        }

        cv::imshow("Display", frame);
        char key = (char)cv::waitKey(30);
        if (key == 'q' || key == 27) {
            break;
        }
        else if (key == 'p') {
            privacyToggle = !privacyToggle;
        }
        else if (key == 's') {
            cv::imwrite("Test.jpg", frame);
        }
    }

    capture.release();
    cv::destroyAllWindows();

    return 0;
}