#ifndef POSEESTIMATION_H
#define POSEESTIMATION_H
#include "Image.h"
#include "FeatureMatch.h"
#include "CameraCompute.h"
#include"../Config.h"
class PoseEstimation{
public:
    PoseEstimation(std::vector<std::string>filePath,Config& config):filePath_(filePath),config_(config){}
    std::vector<ImagePtr> work();
private:
    std::vector<std::string>filePath_;
    Config config_;
    FeatureMatch* featureMatch;
    CameraCompute* cameraCompute;
};
 std::vector<ImagePtr> PoseEstimation::work(){
        if(filePath_.size()!=2){
            std::cout<<"只支持输入两个视图"<<std::endl;
        }
        std::vector<ImagePtr>Images(2);
        for(int i=0;i<2;++i){
            Images[i]=std::make_shared<Image>(filePath_[i],i);
        }
        if(Images[0]->ImgDataRGB.empty()){
            std::cout<<"empty"<<std::endl;
        }
        featureMatch=new FeatureMatch(config_);
        featureMatch->work(Images);
        cameraCompute=new CameraCompute(config_.KFilePath,config_.calibration);
        cameraCompute->work(Images);
        return Images;
    }
#endif