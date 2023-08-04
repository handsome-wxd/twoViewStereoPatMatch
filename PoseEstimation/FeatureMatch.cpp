#include "FeatureMatch.h"
void FeatureMatch::InitConfig(){
    SiftDector=cv::SIFT::create(featurePointNum_);
    fbmatcher=cv::FlannBasedMatcher::create();
    return ;
}
void FeatureMatch::featureExtractSIFT(ImagePtr&  image){
    // if(image->ImgDataRGB.empty())
    //     std::cout<<"没有读入数据"<<std::endl;

    SiftDector->detect(image->ImgDataRGB,image->featurePoints);
    SiftDector->compute(image->ImgDataRGB,image->featurePoints,image->featureDescribes);
    return ;
}
void FeatureMatch::featureMatch(ImagePtr&  image1,ImagePtr&  image2,std::vector<cv::DMatch>& goodMatch){
     
    featureExtractSIFT(image1);
    featureExtractSIFT(image2);
    std::vector<std::vector<cv::DMatch>>Match;
    // std::cout<<image1->featurePoints.size()<<std::endl;
    fbmatcher->knnMatch(image1->featureDescribes,image2->featureDescribes,Match,canditionNum_);
    // std::cout<<canditionNum_<<std::endl;
    // std::cout<<distanceThreshold_<<std::endl;
    //用距离进行初步过滤
    for (size_t k = 0; k< Match.size();k++)
    {
    //只有当最接近匹配和次邻近匹配差距足够大，才说明此时的特征点是一个不错的特征点
    if (Match[k][0].distance < Match[k][1].distance*distanceThreshold_){
        goodMatch.emplace_back(Match[k][0]);
      }
    }
    
    return ;
}

void FeatureMatch::computeFMatrix(ImagePtr& image1,ImagePtr&  image2){
    std::vector<cv::DMatch> goodMatch;
    featureMatch(image1,image2,goodMatch);
     //得到匹配点的坐标
    std::vector<cv::Point2d> obj;
    std::vector<cv::Point2d> scene;
    // std::cout<<"goodMatch.size() ="<<goodMatch.size()<<std::endl;
    for (size_t k = 0; k < goodMatch.size(); ++k)
      {
        obj.emplace_back(image1->featurePoints[goodMatch[k].queryIdx].pt);
        scene.emplace_back(image2->featurePoints[goodMatch[k].trainIdx].pt);
      }
    std::vector<uchar>inlinersOS,inlinersSO;
    image1->F=cv::findFundamentalMat(obj, scene, inlinersOS, cv::USAC_MAGSAC,3);
    image2->F=cv::findFundamentalMat(scene,obj, inlinersSO, cv::USAC_MAGSAC,3);
    //使用F矩阵使用对体几何约束再次进行过滤
    std::vector<cv::DMatch> ransacMatch;
    std::vector<cv::Point2d> filterobj;
    std::vector<cv::Point2d> filterscene;
    for(size_t m=0;m<goodMatch.size();++m){
        if(inlinersOS[m] && inlinersSO[m]){
            ransacMatch.emplace_back(goodMatch[m]);
            filterobj.emplace_back(image1->featurePoints[goodMatch[m].queryIdx].pt);
            filterscene.emplace_back(image2->featurePoints[goodMatch[m].trainIdx].pt);
        }
    }
    image1->filterFeaturePoints=std::move(filterobj);
    image2->filterFeaturePoints=std::move(filterscene);
   
    return ;
}
void FeatureMatch::work(std::vector<ImagePtr>&Images){ 
   
    int imgNum=Images.size();
    for(int i=0;i<imgNum;++i){
        for(int j=i+1;j<imgNum;++j){
              
            computeFMatrix(Images[i],Images[j]);
        }
    }
    
    return ;
}