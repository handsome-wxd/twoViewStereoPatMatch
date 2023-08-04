#include "CameraCompute.h"
#include<fstream>
#include<iostream>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
using namespace cv;
using namespace std;

void CameraCompute::work(std::vector<ImagePtr>&Images){
    if(Images.size()!=2){
        std::cout<<"输入视图个数为"<<Images.size()<<"目前只支持双目视角的立体重建"<<std::endl;
    }
    cv::Mat k;
    if(calibration_){
        //进行相机标定
        k=cameraCalibration(KFilePath_);
    }
    else {
        k=readK(KFilePath_);
    }
    cout<<"k="<<k<<endl;
    k.convertTo(k,CV_64FC1);
    cv::Mat E=k.t()*Images[0]->F*k;
    cv::Mat R,T;
    cv::recoverPose(E,Images[0]->filterFeaturePoints,Images[1]->filterFeaturePoints,k,R,T);
    //  std::cout<<R.type()<<std::endl;
  
    Images[0]->cameraCpu_=new CameraCpu(0,k,cv::Mat::eye(3,3,CV_32FC1),cv::Mat::zeros(3,1,CV_32FC1));
    Images[1]->cameraCpu_=new CameraCpu(1,k,R,T);
    // cout<<"R"<<R<<endl;
    //  cout<<"T"<<T<<endl;
}
bool is_file(const string& filename) {
        struct stat   buffer;
        return (stat (filename.c_str(), &buffer) == 0 && S_ISREG(buffer.st_mode));
    }
void getFileNames(const std::string& path,std::vector<std::string>& filenames){
    DIR *pDir;
    struct dirent* ptr;
    if(!(pDir = opendir(path.c_str())))
        return;
    while((ptr = readdir(pDir))!=0) {
        if (strcmp(ptr->d_name, ".") != 0 && strcmp(ptr->d_name, "..") != 0){
            if(is_file(path + "/" + ptr->d_name))
                 filenames.emplace_back(path + "/" + ptr->d_name);
        }
           
    }
    closedir(pDir);
}
int readImg(const std::string &inputPath,std::vector<cv::Mat>&ImgData){
    std::vector<std::string>fileAddrs;
    getFileNames(inputPath,fileAddrs);
    // vector<cv::Mat>data;
    // printf("fileAddress=%d\n",fileAddrs.size())
    if(fileAddrs.size()<3){
        std::cout<<"使用的是张友正相机标定法，因此输入图片至少大于三张"<<std::endl;
        exit(-1);
    }
    int index=0;
    for(string fileAddr:fileAddrs){
        cv::Mat img=cv::imread(fileAddr,IMREAD_COLOR);
        if(img.empty()){
            std::cout<<"读入图片"<<fileAddr<<"信息为空:"<<std::endl;
            exit(-1);
        }
        ImgData.emplace_back(img);
    }
    return 0;
}
//标定相机内参数
cv::Mat CameraCompute::cameraCalibration(const std::string& filePath){
    std::vector<cv::Mat>imgs;
    int ret=readImg(filePath,imgs);
    printf("imgs.size()",imgs.size());
    int boardWidth = 7;  // 棋盘格横向内角点数量
    int boardHeight = 7; // 棋盘格纵向内角点数量
    float squareSize = 1.f; // 棋盘格格子的大小，单位为米,随便设置，不影响相机内参计算
    Size boardSize(boardWidth, boardHeight);
    vector<vector<Point3f>> objectPoints;
    vector<vector<Point2f>> imagePoints;
    vector<Point2f> corners;
    // 2. 拍摄棋盘图像
    Mat image, gray;
    namedWindow("image", WINDOW_NORMAL);
    for (size_t i = 0; i < imgs.size(); i++)
    {
        // image = imread(fileNames[i], IMREAD_COLOR);
       
        image=imgs[i];
        
        cvtColor(image, gray, COLOR_BGR2GRAY);
        // imshow("image", image);
        //     waitKey();
        // 3. 读入图像数据，并提取角点
        bool found =cv::findChessboardCorners(image, boardSize, corners, CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE + CALIB_CB_FAST_CHECK);
        if (found)
        {
            cornerSubPix(gray, corners, Size(11, 11), Size(-1, -1), TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 30, 0.1));
            drawChessboardCorners(image, boardSize, corners, found);
         

            vector<Point3f> objectCorners;
            for (int j = 0; j < boardHeight; j++)
            {
                for (int k = 0; k < boardWidth; k++)
                {
                    objectCorners.push_back(Point3f(k * squareSize, j * squareSize, 0));
                }
            }
            objectPoints.push_back(objectCorners);
            imagePoints.push_back(corners);
        }
    }

    // 4. 标定相机
    Mat cameraMatrix, distCoeffs;
    vector<Mat> rvecs, tvecs;
    calibrateCamera(objectPoints, imagePoints, image.size(), cameraMatrix, distCoeffs, rvecs, tvecs);
    return cameraMatrix;
   
}
cv::Mat CameraCompute::readK(const std::string& filePath){
  std::ifstream infile;

  cv::Mat Matrix(3,3,CV_32FC1);
  infile.open(filePath,ios::in);
    if (!infile)
    {
        cout << "无法打开文件！" << endl;
        exit(1);
    }
  std::string value;
  for(int i=0;i<3;++i){
    float* ptr=Matrix.ptr<float>(i);
    for(int j=0;j<3;++j){
        infile>>value;
        // cout<<value<<endl;
        ptr[j]=stoi(value);
    }
  }
  return Matrix;
 
}