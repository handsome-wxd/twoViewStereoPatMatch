#ifndef IMG_H
#define IMG_H
#include "CameraCpu.h"
class Image;
typedef std::shared_ptr<Image> ImagePtr;
class Image{
public:
    Image(std::string& filePath,int imgId):imgId_(imgId),rows(0),cols(0),cameraCpu_(nullptr){
        readData(filePath);
    }
    void readData(std::string& filePath){
        ImgDataRGB=cv::imread(filePath,cv::IMREAD_COLOR);
        ImgDataGray=cv::imread(filePath,cv::IMREAD_GRAYSCALE);
        if(ImgDataRGB.empty())
            std::cout<<"未读入图像数据"<<std::endl;
        rows=ImgDataRGB.rows;
        cols=ImgDataGray.cols;
    }
    int imgId_;
    CameraCpu* cameraCpu_;
    cv::Mat ImgDataRGB;
    cv::Mat ImgDataGray;
    cv::Mat DepthMap;
    int rows;
    int cols;
    //特征描述符 特征描述子
    std::vector<cv::KeyPoint>featurePoints;
    cv::Mat featureDescribes;
    std::vector<cv::Point2d>filterFeaturePoints;
    cv::Mat F;
    // std::
    ~Image(){
        delete cameraCpu_;
    }
};
#endif