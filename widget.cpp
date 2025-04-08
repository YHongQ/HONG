#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->timer = new QTimer(this);// 初始化定时器
    QString OutputName = "output";
    ui->SavePathLineEdit->setText(OutputName);
    ui->SavePathLineEdit->setEnabled(false);//保存模型路径框锁定
    // 初始化一些默认参数
    ui->TrainModelHeightLineEdit->setText(QString::number(INPUTHEIGHT));
    ui->TrainModelWidthtLineEdit->setText(QString::number(INPUTWIDTH));
    ui->ClassThresholdWidthtLineEdit->setText(QString::number(CLASSTHRESHOLD));
    ui->NMSThresholdHeightLineEdit->setText(QString::number(NMSTHRESHOLD));
    // 选择图片路径建立链接
    // connect(ui->ImagePathQToolButton,&QToolButton::clicked,this,&Widget::on_ImagePathQToolButton_triggered,Qt::UniqueConnection);
    // connect(ui->VideoPathQToolButton,&QToolButton::clicked,this,&Widget::on_VideoPathQToolButton_clicked,Qt::UniqueConnection);
    connect(this->timer,&QTimer::timeout,this,&Widget::updateFrame,Qt::UniqueConnection);
    // connect(ui->checkBox,&QCheckBox::stateChanged,this,&Widget::on_checkBox_stateChanged,Qt::UniqueConnection);
    // connect(ui->BeginImageQPushButton,&QPushButton::clicked,this,&Widget::on_BeginImageQPushButton_clicked,Qt::UniqueConnection);
}

Widget::~Widget()
{
    if(this->videoCapture.isOpened())
    {
        this->videoCapture.release();
    }
    delete ui;
}

void Widget::on_ImagePathQToolButton_clicked()
{
    QString ImagePath = QFileDialog::getOpenFileName(this,
                                                     tr("Open Image"),
                                                     "D:/CODE/MyUndergraduateDesign/Images/",//
                                                     tr("Image Files(*.png *.jpg *.bmp *.jpeg)"));
    if(!ImagePath.isEmpty())
    {
        ui->ImagePathLineEdit->setText(ImagePath);
        // 加载图像并显示在 QLabel 中
        QPixmap image(ImagePath);
        if (!image.isNull()) {
            ui->ImageLabel->setPixmap(image.scaled(ui->ImageLabel->size(), Qt::KeepAspectRatio));
        } else {
            ui->ImageLabel->setText("Failed to load image!");
        }
    }
}
// void Widget::on_ImagePathQToolButton_triggered()
// {
//     QString ImagePath = QFileDialog::getOpenFileName(this,
//                                                      tr("Open Image"),
//                                                      "D:/CODE/MyUndergraduateDesign/Images/",//
//                                                      tr("Image Files(*.png *.jpg *.bmp *.jpeg)"));
//     if(!ImagePath.isEmpty())
//     {
//         ui->ImagePathLineEdit->setText(ImagePath);
//         // 加载图像并显示在 QLabel 中
//         QPixmap image(ImagePath);
//         if (!image.isNull()) {
//             ui->ImageLabel->setPixmap(image.scaled(ui->ImageLabel->size(), Qt::KeepAspectRatio));
//         } else {
//             ui->ImageLabel->setText("Failed to load image!");
//         }
//     }

// }


void Widget::on_VideoPathQToolButton_clicked()
{
    QString VideoPath = QFileDialog::getOpenFileName(this,
                                                     tr("Open Video"),
                                                     "D:/CODE/MyUndergraduateDesign/",//
                                                     tr("Video Files(*.mp4)"));
    if(!VideoPath.isEmpty())
    {
        ui->VideoPathLineEdit->setText(VideoPath);//将文本框进行更新
        // 获取视频路径文本框内字符串，并将其转化为stdstring ,用于读取
        this->videoCapture.release();
        this->videoCapture.open((ui->VideoPathLineEdit->text()).toStdString());
        std::cout << (ui->VideoPathLineEdit->text()).toStdString()<<std::endl;
        // 加载图像并显示在 QLabel 中
        if(!this->videoCapture.isOpened())
        {
            qDebug()<<"Failed to open video !";
            return;
        }
        if (!timer->isActive()) {
            timer->start(50); // 33ms 更新一帧（约 30 FPS）
        }

    }
}


void Widget::updateFrame()
{
    cv::Mat frame;
    if (videoCapture.read(frame)) {
        // 将 OpenCV 的 cv::Mat 转换为 QImage
        QImage qImage(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_BGR888);

        // 将 QImage 转换为 QPixmap 并显示在 QLabel 中
        ui->ImageLabel->setPixmap(QPixmap::fromImage(qImage).scaled(ui->ImageLabel->size(), Qt::KeepAspectRatio));
    } else {
        // 视频播放结束，停止定时器
        this->timer->stop();
        qDebug() << "Video playback finished!";
    }
}


void Widget::on_ModelPathQTooLButton_clicked()
{
    QString ModelPath = QFileDialog::getOpenFileName(this,
                                                     tr("Open Model"),
                                                     "D:/CODE/MyUndergraduateDesign/",//
                                                     tr("Model Files(*.onnx)"));
    if(!ModelPath.isEmpty())
    {
        ui->ModelPahtLineEdit->setText(ModelPath);
    }
}


void Widget::on_checkBox_stateChanged(int arg1)
{
    ui->SavePathLineEdit->setEnabled(arg1 == Qt::Checked);//当选择是否保存按键被触发，引出模型路径保存的编辑
}


void Widget::on_BeginImageQPushButton_clicked()
{
    ui->BeginImageQPushButton->blockSignals(true);
    std::string imagePath = ui->ImagePathLineEdit->text().toStdString();
    std::string ModelPath = ui->ModelPahtLineEdit->text().toStdString();
    std::string SaveName = ui->SavePathLineEdit->text().toStdString();
    SaveName = SaveName +".png";

    if(!QFile::exists(QString::fromStdString(imagePath)))
    {
        QMessageBox::warning(this,"Error","Image path does not exist");
        ui->BeginImageQPushButton->blockSignals(false);
        return ;
    }
    if(!QFile::exists(QString::fromStdString(ModelPath)))
    {
        QMessageBox::warning(this,"Error","Model path does not exist!");
        ui->BeginImageQPushButton->blockSignals(false);
        return ;
    }
    int inputwidth = ui->TrainModelWidthtLineEdit->text().toInt();
    int inputHeight = ui->TrainModelHeightLineEdit->text().toInt();
    float classThreshold = ui->ClassThresholdWidthtLineEdit->text().toFloat();
    float nmsThreshold = ui->NMSThresholdHeightLineEdit->text().toFloat();
    yolov8_obb opencv_obb(ModelPath,classThreshold,nmsThreshold,inputwidth,inputHeight);
    bool IsSaveResult = ui->checkBox->isChecked();
    opencv_obb.DetectOneImage(imagePath,IsSaveResult,SaveName);
    ui->BeginImageQPushButton->blockSignals(false);
}





void Widget::on_BeginVideoQPushButton_clicked()
{
    ui->BeginVideoQPushButton->blockSignals(true);
    std::string VideoPath = ui->VideoPathLineEdit->text().toStdString();
    std::string ModelPath = ui->ModelPahtLineEdit->text().toStdString();
    std::string SaveName = ui->SavePathLineEdit->text().toStdString();
    SaveName = SaveName +".mp4";

    if(!QFile::exists(QString::fromStdString(VideoPath)))
    {
        QMessageBox::warning(this,"Error","The Video path is not exist !");
        ui->BeginVideoQPushButton->blockSignals(false);
        return ;
    }
    if (!QFile::exists(QString::fromStdString(ModelPath)))
    {
        QMessageBox::warning(this,"Error","The Model path is not exist !");
        ui->BeginVideoQPushButton->blockSignals(false);
        return ;
    }

    int inputwidth = ui->TrainModelWidthtLineEdit->text().toInt();
    int inputHeight = ui->TrainModelHeightLineEdit->text().toInt();
    float classThreshold = ui->ClassThresholdWidthtLineEdit->text().toFloat();
    float nmsThreshold = ui->NMSThresholdHeightLineEdit->text().toFloat();
    yolov8_obb opencv_obb(ModelPath,classThreshold,nmsThreshold,inputwidth,inputHeight);
    bool IsSaveResult = ui->checkBox->isChecked();
    opencv_obb.DetectVideo(VideoPath,IsSaveResult,SaveName.c_str());
    ui->BeginVideoQPushButton->blockSignals(false);
}

