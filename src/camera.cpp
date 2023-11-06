//
// Created by ning on 23-9-27.
//
#include "camera.h"

camera::camera() {
    cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M','J','P','G'));
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640); //图像的宽，需要相机支持此宽
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480); //图像的高，需要相机支持此高

    deviceID = 0;

    cap.open(deviceID);

    if(!cap.isOpened()) {
        std::cerr << "ERROR!! Unable to open camera\n";
    }
}

cv::Mat camera::getImage() {
    cv::Mat img;

    cap >> img; //以流形式捕获图像

    cv::namedWindow("example", 1); //创建一个窗口用于显示图像，1代表窗口适应图像的分辨率进行拉伸。
    if (img.empty()) //图像不为空则显示图像
    {
        std::cerr << "ERROR!! No image!!";
    }
    return img;
}

camera::~camera() {
    cap.release();
}