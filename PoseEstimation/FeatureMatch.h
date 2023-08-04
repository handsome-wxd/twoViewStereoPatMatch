#ifndef FEATUREMATCH_H
#define FEATUREMATCH_H
#include "Image.h"
#include"../Config.h"
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/xfeatures2d.hpp>
typedef std::shared_ptr<Image> ImagePtr;
class FeatureMatch{
public:
    FeatureMatch(Config& config):
    featurePointNum_(config.featurePointNum),canditionNum_(config.canditionNum),distanceThreshold_(config.distanceThreshold){
        InitConfig();
    }
    void InitConfig(); 
    void work(std::vector<ImagePtr>&Images);
    void featureExtractSIFT(ImagePtr&  image);
    void featureMatch(ImagePtr&  image1,ImagePtr&  image2,std::vector<cv::DMatch>& goodMatch);
    void computeFMatrix(ImagePtr& image1,ImagePtr&  image2);
    int featurePointNum_;
    int canditionNum_;
    float distanceThreshold_;
    cv::Ptr<cv::SIFT> SiftDector;
    cv::Ptr<cv::FlannBasedMatcher> fbmatcher;
};

#endif