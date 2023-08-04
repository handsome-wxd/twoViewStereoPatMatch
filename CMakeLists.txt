cmake_minimum_required(VERSION 2.6)
project(TwoViewStereoPathMatch)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")
FIND_PACKAGE(CUDA REQUIRED)
FIND_PACKAGE(OpenCV REQUIRED)
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CUDA_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/PoseEstimation)
include_directories(${PROJECT_SOURCE_DIR}/DenseMatch)
include_directories(config.h)
AUX_SOURCE_DIRECTORY(PoseEstimation  PoseEstimation_DIR_VAR)

cuda_add_library(
    DenseMatch
    DenseMatch/DenseMatch.cu
    DenseMatch/UtilGpu.cu
    DenseMatch/PatchMatchGpu.cu
    

)
add_executable(TwoViewStereoPathMatch main.cpp ${PoseEstimation_DIR_VAR})
# 与Ceres和OpenCV链接
target_link_libraries(TwoViewStereoPathMatch ${OpenCV_LIBS} DenseMatch)