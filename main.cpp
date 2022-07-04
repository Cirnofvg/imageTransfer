#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //a.setWindowIcon(QIcon("D:/Work/image_transfer/pictures/test.png"));
    MainWindow w;
    w.setWindowTitle("Image Transfer");
    w.setWindowIcon(QIcon(":/new/prefix1/icon"));
    w.show();
    return a.exec();
}
