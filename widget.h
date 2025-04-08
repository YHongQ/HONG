#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
// #include "QString"
#include <QPushButton>
#include <QLineEdit>
#include <QFileDialog>
#include <QtCore/QString>
#include <QPixmap>
#include <opencv2/opencv.hpp>
#include <QTimer>
#include <QMessageBox>
#include "yolov8_obb.h"
#include "yolov8_utils.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:

    // //选择图片路径槽函数
    // void on_ImagePathQToolButton_triggered();
    // 选择视频路径槽函数
    void on_VideoPathQToolButton_clicked();
    // 选择模型路径槽函数
    // 测试窗口动态更新，使用定时器
    void updateFrame();
    // 选择模型路径槽函数
    void on_ModelPathQTooLButton_clicked();
    // 保存检测结果选项是否被勾选
    void on_checkBox_stateChanged(int arg1);
    // 开始检测按键
    void on_BeginImageQPushButton_clicked();

    void on_ImagePathQToolButton_clicked();

    void on_BeginVideoQPushButton_clicked();

private:
    Ui::Widget *ui;
    cv::VideoCapture videoCapture;
    QTimer *timer;
};
#endif // WIDGET_H
