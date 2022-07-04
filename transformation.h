#ifndef Transformation_H
#define Transformation_H

#include <QImage>
#include <QLabel>

class Transformation
{
public:
    Transformation();
    static void RGBtoHSV (QImage &image);
    static void HSVtoRGB (QImage &image);
    static void RGBtoYUV (QImage &image);
    static void YUVtoRGB (QImage &image);
    static void RGBtoYCbCr (QImage &image);
    static void YCbCrtoRGB (QImage &image);
    static void RGBtoBGR (QImage &image);
    static void BGRtoRGB (QImage &image);
    static void RGBtoXYZ (QImage &image);
    static void XYZtoRGB (QImage &image);
    static void RGBtoYCC (QImage &image);
    static void YCCtoRGB (QImage &image);
    static void RGBtoHLS (QImage &image);
    static void HLStoRGB (QImage &image);
    static void RGBtoGray (QImage &image);
    static void MedianFilter (QImage &image);
    static void MedianFilterSquare (QImage &image);
    static QString transformTag;
};

#endif // Transformation_H
