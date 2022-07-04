#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void update();

private slots:
    void on_actionOpen_file_triggered();

    void on_actionExit_triggered();

    void on_actionRGtoHSV_triggered();

    void on_actionHSVtoRGB_triggered();

    void on_actionRGBtoYUV_triggered();

    void on_actionYUVtoRGB_triggered();

    void on_actionRGBtoYCbCr_triggered();

    void on_actionYCbCrtoRGB_triggered();

    void on_actionRGBtoBGR_triggered();

    void on_actionBGRtoRGB_triggered();

    void on_actionRGBtoXYZ_triggered();

    void on_actionXYZtoRGB_triggered();

    void on_actionRGBtoYCC_triggered();

    void on_actionYCCtoRGB_triggered();

    void on_actionRGBtoHLS_triggered();

    void on_actionHLStoRGB_triggered();


    void on_actionRGBtoGray_triggered();

    void on_actionSave_triggered();

    void on_actionReset_triggered();

    void on_actionMedian_Filter_triggered();

    void on_actionRight_triggered();

    void on_actionLeft_triggered();

    void on_action180_triggered();

    void on_actionMedian_Filter_5x5_triggered();

    void on_actionRotate_on_triggered();

private:
    QString fileName;
    Ui::MainWindow *ui;
    QImage image;
    float scale = 1.f;

protected:
    void wheelEvent(QWheelEvent *ev);
    void resizeEvent(QResizeEvent *event);
};
#endif // MAINWINDOW_H
