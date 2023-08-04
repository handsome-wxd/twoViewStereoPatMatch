#ifndef CAMERACOMPUTE_H
#define CAMERACOMPUTE_H
#include <iostream>
#include <opencv2/opencv.hpp>
#include "Image.h"

class CameraCompute{
public:
    CameraCompute(std::string& KFilePath,bool calibration):KFilePath_(KFilePath),calibration_(calibration){}
    cv::Mat cameraCalibration(const std::string& filePath);
    cv::Mat readK(const std::string& filePath);
    void work(std::vector<ImagePtr>&Images);
    std::string KFilePath_;
    bool calibration_;
    
};
#endif