#ifndef CONFIG_H
#define CONFIG_H
#include<iostream>
class Config{
 public:
    //位姿估计
    int featurePointNum=10000;
    int canditionNum=2;
    float distanceThreshold=0.8;
    //已知相机内参
    // std::string KFilePath="../data/img1/K/K.txt";//相机内参目录
    // bool calibration=false;
    //未知相机内参
    std::string KFilePath="../data/photo";//标定图片目录
    bool calibration=true;
};
#endif