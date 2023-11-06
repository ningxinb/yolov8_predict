//
// Created by ning on 23-9-27.
//

#ifndef YOLOV8_ORT_CAMERA_H
#define YOLOV8_ORT_CAMERA_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include<sstream>
class camera{
public:
    camera();
    cv::Mat getImage();
    ~camera();
private:
    cv::VideoCapture cap;
    int deviceID;
};
#endif //YOLOV8_ORT_CAMERA_H
