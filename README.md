# HONG
基于OpenCV和Qt的YOLO Detect模型部署

# 环境
### 基于Qt6 和OpenCV 4.9.0（安装Qt默认MSVC编译的即可） 
主要使用 openCV的DNN进行模型的部署推理，在这个任务里，主要是对于车辆的检测，因此针对不同的任务需求需要调整输出格式。
这里输出三维张量 batch*result*8000 .表示批处理和检测结果的（5个类别置信度+OBB框x、y、w、h、angle）。针对不同的任务需求，需要对结果进行调整，但是大体逻辑是一样的。
如果直接在Qt里编译该项目，要将对应的opencv xx.dll 之类的链接库配置好。

![image](https://github.com/user-attachments/assets/208d757b-727f-4f61-a307-90dc82ad0c6f)
![image](https://github.com/user-attachments/assets/0f7c071f-93c5-490e-ae16-febcb9c46464)

## 介绍

#define CLASSTHRESHOLD  0.25  


#define NMSTHRESHOLD  0.45  


#define INPUTWIDTH 640  


#define INPUTHEIGHT 640  


 在这个方法里面对于训练好的模型要采用ONNX格式进行加载，另一个就是数据输入大小，这个里面采用的是640*640大小，如果训练好的模型不是这个尺寸需要修改成自己训练模型的尺寸。 

 
 另一个方面，从.pt导出标准onnx 版本注意选择12 以及是否接受动态输入大小。  

 
 这里实际上是不需要动态输入的，在程序的数据预处理阶段有基于yolov5 的letterBox 进行预处理，在构造和成员函数中控制好大小就行。  


// output : 1*10*8000,数据维度，1*10*8000,1表示batch_size,10表示每个特征图的anchor数，8000表示每个anchor的检测结果


// anchor数据是一个一维数组，包含了检测结果的所有信息，每个anchor的数据格式为：[x,y,w,h,confidence,class1,class2,class3,class4,class5]

       
另一个参数的设定，程序里通过宏定义实际上有一组默认的参数，但是对于检测时候我们想要修改，槽函数的设计部分有检查对应组件的数值，调用检测的成员函数进行修改值，这样就可以完成对应的检验并修改成员值。


## 针对不同的任务
如果任务不是OBB检测，而是其他检测， 修改Detect函数，控制好输出格式，将数据读取即可，另一个地方就是检查绘制框体部分，那个地方主要是对OBB或是bndbox的绘制，因为两者完全有些差异，要调整好传入的数据。


ui设计的话，主要有个定时器的槽函数进行更新界面里视频或是图片的更新，检测结果的话，是调用opencv直接另开一个窗口就行。



## 参考文章
https://github.com/TangTao-PKU/YOLOv8-ONNXRuntime-CPP/blob/main/inference.h


https://github.com/UNeedCryDear/yolov8-opencv-onnxruntime-cpp/tree/main

建议参考第二个文章的内容，他已经更新了针对不同任务的检测，可以下载下来进行实现。
