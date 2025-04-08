#ifndef YOLOV8_OBB_H
#define YOLOV8_OBB_H
#include<iostream>
#include<opencv2/opencv.hpp>
#include<cstdio>
#include"yolov8_utils.h"

#define CLASSTHRESHOLD  0.25
#define NMSTHRESHOLD  0.45
#define INPUTWIDTH 640
#define INPUTHEIGHT 640
class yolov8_obb
{
public:
    yolov8_obb() {};
    yolov8_obb(const std::string model_path);
    yolov8_obb(const std::string model_path,float classThreshold,float numsThreshold,int inputWidth,int inputHeight);
    ~yolov8_obb() {};
    // 读取模型
    bool ReadModel(const std::string model_path);
    // 检测OBB框体，检测结果保存在outputParams中
    bool Detect(cv::Mat& img, std::vector<OutputParams>& outputParams);
    // 检测视频流数据
    bool DetectVideo(std::string video_path, bool IsSaveVideo = false, const char* SaveVideoPath="output.mp4");
    bool DetectOneImage(std::string image_path, bool IsSaveImage = false, std::string SaveImagePath = "output.jpg");
    std::vector<std::string> _classes = {"car","bus","truck","feright_car","van"};//类别名称
    // 0:car, 1:bus, 2:truck, 3:feright_car, 4:van .对应5个类别
    int _num_classes = _classes.size();//类别数
    cv::dnn::Net _net;//ONNX模型
private:
    float _classThreshold = CLASSTHRESHOLD;//类别阈值,用来过滤低于阈值的类别
    float _nmsThreshold = NMSTHRESHOLD;//NMS阈值,用来过滤重叠度高的检测框
    int _inputWidth = INPUTWIDTH;//ONNX模型输入的宽度
    int _inputHeight = INPUTHEIGHT;//ONNX模型输入的高度
};



#endif // YOLOV8_OBB_H
