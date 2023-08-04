#ifndef CAMERACPU_H
#define CAMERACPU_H
#include<opencv2/opencv.hpp>
#include<iostream>
class CameraCpu{
public:
    CameraCpu(int camerId,cv::Mat K,cv::Mat R,cv::Mat T):
    camerId_(camerId),K_(K),R_(R),T_(T){
        computeOptionCenter();
    }
    void computeOptionCenter(){
         OptionCenter_=-R_.inv()*T_;
             return ;
    };
    int camerId_;
    cv::Mat K_;
    cv::Mat R_;
    cv::Mat T_;
    cv::Mat OptionCenter_;
};

#endif