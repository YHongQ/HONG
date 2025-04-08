#include "widget.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <opencv2/opencv.hpp>
// #include"yolov8_obb.h"
// #include"yolov8_utils.h"

// void test_video()
// {

//     std::string video_path = "D:\\CODE\\MyUndergraduateDesign\\Video\\uav305\\output_video.mp4";
//     std::string model_path = "D:/CODE/MyUndergraduateDesign/Models/best_640640_10_8400.onnx";
//     cv::dnn::Net net;// 初始化网络
//     yolov8_obb obb_opencv(model_path);
//     //obb_opencv.ReadModel(model_path);
//     obb_opencv.DetectVideo(video_path,false);
// };
// void test_image()
// {
//     yolov8_obb obb_opencv;
//     std::string filename = "D:/CODE/MyUndergraduateDesign/Images/00125.jpg";
//     std::string model_path = "D:/CODE/MyUndergraduateDesign/Models/best_640640_10_8400.onnx";
//     obb_opencv.ReadModel(model_path);
//     obb_opencv.DetectOneImage(filename,false);
// }

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "firstproject_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    Widget w;
    w.show();
    return a.exec();
}
