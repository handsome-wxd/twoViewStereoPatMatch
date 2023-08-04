#include"PoseEstimation/PoseEstimation.h"
#include"Config.h"
#include "DenseMatch/DenseMatch.h"
int main(){
    std::vector<std::string>filePath(2);
    filePath[0]="../data/img9/4876_1682317645_hd.jpg";
    filePath[1]="../data/img9/4877_1682317647_hd.jpg";
    Config* config=new Config();
    PoseEstimation* poseEstimation=new  PoseEstimation(filePath,*config);
    std::vector<ImagePtr>Images=poseEstimation->work();
    DenseMatch(Images);
    delete config;
    delete poseEstimation;
    return 0;
}
