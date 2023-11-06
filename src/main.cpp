#include <regex>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <filesystem>
#include <ctime>

#include "utils.h"
#include "predictor.h"
#include "camera.h"

int main(int argc, char *argv[])
{
    float confThreshold = 0.4f;
    float iouThreshold = 0.4f;

    float maskThreshold = 0.5f;

//    cmdline::parser cmd;
//    cmd.add<std::string>("model_path", 'm', "Path to onnx model.", false, "yolov8m.onnx");
//    cmd.add<std::string>("image_path", 'i', "Image source to be predicted.", false, "./Imginput");
//    cmd.add<std::string>("out_path", 'o', "Path to save result.", false, "./Imgoutput");
//    cmd.add<std::string>("class_names", 'c', "Path to class names file.", false, "coco.names");
//
//    cmd.add<std::string>("suffix_name", 'x', "Suffix names.", false, "yolov8m");
//
//    cmd.add("gpu", '\0', "Inference on cuda device.");
//
//    cmd.parse_check(argc, argv);

    bool isGPU = true;
    const std::string classNamesPath = "../models/coco.names";
    const std::vector<std::string> classNames = utils::loadNames(classNamesPath);
    const std::string imagePath = "../image";
    const std::string savePath = "../output";
    const std::string suffixName = "yolov8m";
    const std::string modelPath = "../models/yolov8n-seg.onnx";

    if (classNames.empty())
    {
        std::cerr << "Error: Empty class names file." << std::endl;
        return -1;
    }
    if (!std::filesystem::exists(modelPath))
    {
        std::cerr << "Error: There is no model." << std::endl;
        return -1;
    }
    if (!std::filesystem::is_directory(imagePath))
    {
        std::cerr << "Error: There is no imagePath." << std::endl;
        return -1;
    }
    if (!std::filesystem::is_directory(savePath))
    {
        std::filesystem::create_directory(savePath);
    }
    std::cout << "Model from :::" << modelPath << std::endl;
    std::cout << "Images from :::" << imagePath << std::endl;
    std::cout << "Resluts will be saved :::" << savePath << std::endl;

    YOLOPredictor predictor{nullptr};
    try
    {
        predictor = YOLOPredictor(modelPath, isGPU,
                                  confThreshold,
                                  iouThreshold,
                                  maskThreshold);
        std::cout << "Model was initialized." << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    assert(classNames.size() == predictor.classNums);
    std::regex pattern(".+\\.(jpg|jpeg|png|gif)$");
    std::cout << "Start predicting..." << std::endl;

    clock_t startTime, endTime;
    startTime = clock();

    int picNums = 0;
    camera cam;
    cv::namedWindow("example", 1);
    while(1) {
        clock_t startTime1, endTime1;
        startTime1 = clock();
        cv::Mat image = cam.getImage();
        std::vector<Yolov8Result> result = predictor.predict(image);
        utils::visualizeDetection(image, result, classNames);
        cv::imshow("example", image);
        endTime1 = clock();
        std::cout << "The segment time is: " << (double)(endTime1 - startTime1) / CLOCKS_PER_SEC << "seconds" << std::endl;
        std::cout << CLOCKS_PER_SEC << std::endl;
        int  key = cv::waitKey(30); //等待30ms
        if (key ==  int('q')) //按下q退出
        {
            break;
        }
    }
//    for (const auto &entry : std::filesystem::directory_iterator(imagePath))
//    {
//        clock_t startTime1, endTime1;
//        startTime1 = clock();
//        if (std::filesystem::is_regular_file(entry.path()) && std::regex_match(entry.path().filename().string(), pattern))
//        {
//            picNums += 1;
//            std::string Filename = entry.path().string();
//            std::string baseName = std::filesystem::path(Filename).filename().string();
//            std::cout << Filename << " predicting..." << std::endl;
//
//            cv::Mat image = cv::imread(Filename);
//            std::vector<Yolov8Result> result = predictor.predict(image);
////            utils::visualizeDetection(image, result, classNames);
//
////            std::string newFilename = baseName.substr(0, baseName.find_last_of('.')) + "_" + suffixName + baseName.substr(baseName.find_last_of('.'));
////            std::string outputFilename = savePath + "/" + newFilename;
////            cv::imwrite(outputFilename, image);
////            std::cout << outputFilename << " Saved !!!" << std::endl;
//        }
//        endTime1 = clock();
//        std::cout << "The segment time is: " << (double)(endTime1 - startTime1) / CLOCKS_PER_SEC << "seconds" << std::endl;
//    }
    endTime = clock();
    std::cout << "The total run time is: " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "seconds" << std::endl;
    std::cout << "The average run time is: " << (double)(endTime - startTime) / picNums / CLOCKS_PER_SEC << "seconds" << std::endl;

    std::cout << "##########DONE################" << std::endl;

    return 0;
}

