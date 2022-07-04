#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "transformation.h"
#include <QWheelEvent>
#include <QTimer>
#include <qinputdialog.h>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSize size = this->size();
    ui->centralwidget->setMaximumSize(size);
    //QImage defaultImage = QImage(":/new/prefix1/icon");
    //ui->imageLabel->setPixmap(QPixmap::fromImage(defaultImage));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionOpen_file_triggered()
{
    QString filter = "File Description (*.jpg)";
    // For example: "Mpeg Layer 3 music files (*.mp3)"
    fileName = QFileDialog::getOpenFileName(this, "Select a file...", QDir::homePath(), filter);
    image = QImage(fileName);
    ui->imageLabel->setPixmap(QPixmap::fromImage(image));
    ui->actionSave->setEnabled(true);
    ui->menuTransform_Functions->setEnabled(true);
    ui->menuTransform_Reset->setEnabled(true);
    ui->menuFilters->setEnabled(true);
    ui->menuRotate->setEnabled(true);
    scale = 1.f;
}

void MainWindow::on_actionExit_triggered()
{
    exit(1);
}

void MainWindow::on_actionRGtoHSV_triggered()
{
    Transformation::RGBtoHSV(image);
    update();
    ui->textBrowser->setText("HSV");
}

void MainWindow::on_actionHSVtoRGB_triggered()
{
    Transformation::HSVtoRGB(image);
    update();
    ui->textBrowser->setText("RGB");
}

void MainWindow::on_actionRGBtoYUV_triggered()
{
    Transformation::RGBtoYUV(image);
    update();
    ui->textBrowser->setText("YUV");
}

void MainWindow::on_actionYUVtoRGB_triggered()
{
    Transformation::YUVtoRGB(image);
    update();
    ui->textBrowser->setText("RGB");
}

void MainWindow::on_actionRGBtoYCbCr_triggered()
{
    Transformation::RGBtoYCbCr(image);
    update();
    ui->textBrowser->setText("YCbCr");
}

void MainWindow::on_actionYCbCrtoRGB_triggered()
{
    Transformation::YCbCrtoRGB(image);
    update();
    ui->textBrowser->setText("RGB");
}

void MainWindow::on_actionRGBtoBGR_triggered()
{
    Transformation::RGBtoBGR(image);
    update();
    ui->textBrowser->setText("BGR");
}

void MainWindow::on_actionBGRtoRGB_triggered()
{
    Transformation::BGRtoRGB(image);
    update();
    ui->textBrowser->setText("RGB");
}

void MainWindow::on_actionRGBtoXYZ_triggered()
{
    Transformation::RGBtoXYZ(image);
    update();
    ui->textBrowser->setText("XYZ");
}

void MainWindow::on_actionXYZtoRGB_triggered()
{
    Transformation::XYZtoRGB(image);
    update();
    ui->textBrowser->setText("RGB");
}

void MainWindow::on_actionRGBtoYCC_triggered()
{
    Transformation::RGBtoYCC(image);
    update();
    ui->textBrowser->setText("YCC");
}

void MainWindow::on_actionYCCtoRGB_triggered()
{
    Transformation::YCCtoRGB(image);
    update();
    ui->textBrowser->setText("RGB");
}

void MainWindow::on_actionRGBtoHLS_triggered()
{
    Transformation::RGBtoHLS(image);
    update();
    ui->textBrowser->setText("HLS");
}

void MainWindow::on_actionHLStoRGB_triggered()
{
    Transformation::HLStoRGB(image);
    update();
    ui->textBrowser->setText("RGB");
}


void MainWindow::on_actionRGBtoGray_triggered()
{
    Transformation::RGBtoGray(image);
    update();
    ui->textBrowser->setText("Gray");
}

void MainWindow::on_actionSave_triggered()
{
     QString filter = "File Description (*.jpg)";
     QString fileName = QFileDialog::getSaveFileName(this, "Select where to save...", QDir::homePath(), filter);
     image.save(fileName, nullptr, 100);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    QSize size = this->size();
    size.setHeight(size.height() - 41);
    ui->centralwidget->setMaximumSize(size);
}

void MainWindow::wheelEvent(QWheelEvent *ev)
{
    if(ev->angleDelta().y() > 0) {
        // up Wheel
        scale *= 1.15;
    }
    else {
        //down Wheel
        scale /= 1.15;
    }
    update();
}

void MainWindow::update() {
    float w = image.width();
    float h = image.height();
    QImage image1 = image.scaled(w * scale,h * scale, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    ui->imageLabel->setPixmap(QPixmap::fromImage(image1));
}


void MainWindow::on_actionReset_triggered()
{
    image = QImage(fileName);
    ui->imageLabel->setPixmap(QPixmap::fromImage(image));
    update();
    ui->textBrowser->setText("");
}



void MainWindow::on_actionMedian_Filter_triggered()
{
    Transformation::MedianFilter(image);
    update();
}

void MainWindow::on_actionRight_triggered()
{
    QPixmap pixmap1 = QPixmap::fromImage(image);
    QTransform transform;
    transform.rotate(90);
    pixmap1 = pixmap1.transformed(transform);
    ui->imageLabel->setPixmap(pixmap1);
    image = pixmap1.toImage();
    update();
}

void MainWindow::on_actionLeft_triggered()
{
    QPixmap pixmap1 = QPixmap::fromImage(image);
    QTransform transform;
    transform.rotate(-90);
    pixmap1 = pixmap1.transformed(transform);
    ui->imageLabel->setPixmap(pixmap1);
    image = pixmap1.toImage();
    update();
}

void MainWindow::on_action180_triggered()
{
    QPixmap pixmap1 = QPixmap::fromImage(image);
    QTransform transform;
    transform.rotate(180);
    pixmap1 = pixmap1.transformed(transform);
    ui->imageLabel->setPixmap(pixmap1);
    image = pixmap1.toImage();
    update();
}

void MainWindow::on_actionMedian_Filter_5x5_triggered()
{
    Transformation::MedianFilterSquare(image);
    update();
}

void MainWindow::on_actionRotate_on_triggered()
{
    bool ok;
        QString text = QInputDialog :: getText (this, tr ("Изменение угла"), tr ("Введите угол в градусах"), QLineEdit::Normal, 0, & ok);
        if (ok && !text.isEmpty())
        {
            //int g = QString.toInt(text)
            QPixmap pixmap1 = QPixmap::fromImage(image);
            QTransform transform;
            transform.rotate(text.toInt());
            pixmap1 = pixmap1.transformed(transform);
            ui->imageLabel->setPixmap(pixmap1);
            image = pixmap1.toImage();
            update();
        }
}
