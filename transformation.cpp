#include "Transformation.h"
#include <cmath>
//#include <omp.h>
#include <QTime>
#include <QDebug>
#include <cmath>
#include <algorithm>

Transformation::Transformation()
{
}


void Transformation::RGBtoHSV (QImage &image) {
    QTime time;
    time.start();
#pragma omp parallel for
    for (int y = 0; y < image.height() ; ++y) {
        QRgb *line;
#pragma omp critical
        {
            line = reinterpret_cast<QRgb*>(image.scanLine(y));
        }
        for (int x = 0; x < image.width(); ++x) {
            QRgb &rgb = line[x];
            float S, H;
            float R = qRed(rgb);
            float G = qGreen(rgb);
            float B = qBlue(rgb);
            // Lightness
            float V = std::max(std::max(R, G), B);
            // Saturation
            float temp = std::min(std::min(R, G), B);
            if (V == 0) {
                S = 0;
                H = 0;
            } else {
                S  = (V - temp) / V;
            }
            // Hue
            float Cr, Cg, Cb;
            Cr = (V - R) / (V - temp);
            Cg = (V - G) / (V - temp);
            Cb = (V - B) / (V - temp);
            if (R == V) {H = Cb - Cg;}
            if (G == V) {H = 2.0f + Cr - Cb;}
            if (B == V) {H = 4.0f + Cg - Cr;}
            H = 60.0f * H;
            if (H < 0.0f) {H = H + 360.0f;}

            H = 255.0f * (H / 360.0f);
            S *= 255.0f;

            rgb = qRgba(H, S, V, 255);
        }
    }
    qDebug() << time.elapsed();
}

void Transformation::HSVtoRGB (QImage &image) {
#pragma omp parallel for
    for (int y = 0; y < image.height() ; ++y) {
        QRgb *line;
#pragma omp critical
        {
            line = reinterpret_cast<QRgb*>(image.scanLine(y));
        }
        for (int x = 0; x < image.width(); ++x) {
            QRgb &rgb = line[x];

            float R, G, B, F, M, N, K;
            float H = (qRed(rgb)/255.0f) * 360.0f;
            float S = qGreen(rgb)/255.0f;
            float V = qBlue(rgb)*1.0f;

            if (S == 0) {
                R = V; G = V; B = V;
            } else if (H == 360.0f) {
                H = 0;
            } else {
                H = H/60.0f;
            }

            double I;
            I = floor(H);
            F = H - I;
            M = V * (1 - S);
            N = V * (1 - S * F);
            K = V * (1 - S * (1 - F));
            if (I == 0.0) {
                R = V;
                G = K;
                B = M;
            }
            if (I == 1.0) {
                R = N;
                G = V;
                B = M;
            }
            if (I == 2.0) {
                R = M;
                G = V;
                B = K;
            }
            if (I == 3.0) {
                R = M;
                G = N;
                B = V;
            }
            if (I == 4.0) {
                R = K;
                G = M;
                B = V;
            }
            if (I == 5.0) {
                R = V;
                G = M;
                B = N;
            }

           rgb = qRgba(R, G, B, 255);
        }
    }
}

void Transformation::RGBtoYUV (QImage &image) {

#pragma omp parallel for
    for (int y = 0; y < image.height() ; ++y) {
        QRgb *line;
#pragma omp critical
        {
            line = reinterpret_cast<QRgb*>(image.scanLine(y));
        }
        for (int x = 0; x < image.width(); ++x) {
            QRgb &rgb = line[x];
            float Y, U, V;
            float R = qRed(rgb);
            float G = qGreen(rgb);
            float B = qBlue(rgb);
            Y = R * 0.299f + 0.587f * G + B * 0.114f;
            U = -0.147 * R - 0.289f * G + 0.436f * B;
            V = 0.615f * R - 0.515f * G - 0.100f * B;
            Y = std::fmax(0, std::fmin(255, Y + 16));
            U = std::fmax(0, std::fmin(255, U + 128));
            V = std::fmax(0, std::fmin(255, V + 128));
            rgb = qRgba(Y, U, V, 255);
        }

    }

}

void Transformation::YUVtoRGB (QImage &image) {

#pragma omp parallel for
    for (int y = 0; y < image.height() ; ++y) {
        QRgb *line;
#pragma omp critical
        {
            line = reinterpret_cast<QRgb*>(image.scanLine(y));
        }
        for (int x = 0; x < image.width(); ++x) {
            QRgb &rgb = line[x];
            float R, G, B;
            float Y = qRed(rgb) - 16;
            float U = qGreen(rgb) - 128;
            float V = qBlue(rgb) - 128;
            R = Y + 1.140f * V;
            G = Y - 0.394f * U - 0.581f * V;
            B = Y + 2.032f * U;
            R = std::fmax(0, std::fmin(255, R));
            G = std::fmax(0, std::fmin(255, G));
            B = std::fmax(0, std::fmin(255, B));
            rgb = qRgba(R, G, B, 255);
        }

    }
}

void Transformation::RGBtoYCbCr (QImage &image) {
#pragma omp parallel for
    for (int y = 0; y < image.height() ; ++y) {
        QRgb *line;
#pragma omp critical
        {
            line = reinterpret_cast<QRgb*>(image.scanLine(y));
        }
        for (int x = 0; x < image.width(); ++x) {
            QRgb &rgb = line[x];
            float Y, Cb, Cr;
            float R = qRed(rgb);
            float G = qGreen(rgb);
            float B = qBlue(rgb);
            Y = 0.257 * R + 0.504 * G + 0.098 * B + 16;
            Cb = -0.148 * R - 0.291 * G + 0.439 * B + 128;
            Cr = 0.439 * R - 0.368 * G - 0.071 * B + 128;
            rgb = qRgba(Y, Cb, Cr, 255);
        }
    }
}

void Transformation::YCbCrtoRGB (QImage &image) {
#pragma omp parallel for
    for (int y = 0; y < image.height() ; ++y) {
        QRgb *line;
#pragma omp critical
        {
            line = reinterpret_cast<QRgb*>(image.scanLine(y));
        }
        for (int x = 0; x < image.width(); ++x) {
            QRgb &rgb = line[x];
            float R, G, B;
            float Y = qRed(rgb);
            float Cb = qGreen(rgb);
            float Cr = qBlue(rgb);
            R = 1.164 * (Y - 16) + 1.596 * (Cr - 128);
            G = 1.164 * (Y - 16) - 0.813 * (Cr - 128) - 0.392 * (Cb - 128);
            B = 1.164 * (Y - 16) + 2.017 * (Cb - 128);
            R = std::fmax(0, std::fmin(255, R));
            G = std::fmax(0, std::fmin(255, G));
            B = std::fmax(0, std::fmin(255, B));
            rgb = qRgba(R, G, B, 255);
        }
    }
}

void Transformation::RGBtoBGR (QImage &image) {
#pragma omp parallel for
    for (int y = 0; y < image.height() ; ++y) {
        QRgb *line;
#pragma omp critical
        {
            line = reinterpret_cast<QRgb*>(image.scanLine(y));
        }
        for (int x = 0; x < image.width(); ++x) {
            QRgb &rgb = line[x];
            float R = qRed(rgb);
            float G = qGreen(rgb);
            float B = qBlue(rgb);
            rgb = qRgba(B, G, R, 255);
        }
    }
}

void Transformation::BGRtoRGB (QImage &image) {
#pragma omp parallel for
    for (int y = 0; y < image.height() ; ++y) {
        QRgb *line;
#pragma omp critical
        {
            line = reinterpret_cast<QRgb*>(image.scanLine(y));
        }
        for (int x = 0; x < image.width(); ++x) {
            QRgb &rgb = line[x];
            float B = qRed(rgb);
            float G = qGreen(rgb);
            float R = qBlue(rgb);
            rgb = qRgba(R, G, B, 255);
        }
    }
}

void Transformation::RGBtoXYZ (QImage &image) {
#pragma omp parallel for
    for (int y = 0; y < image.height() ; ++y) {
        QRgb *line;
#pragma omp critical
        {
            line = reinterpret_cast<QRgb*>(image.scanLine(y));
        }
        for (int x = 0; x < image.width(); ++x) {
            QRgb &rgb = line[x];
            float X, Y, Z;
            float R = qRed(rgb);
            float G = qGreen(rgb);
            float B = qBlue(rgb);
            X = 0.412453 * R/255.0 + 0.35758 * G/255.0 + 0.180423 * B/255.0;
            if (X < 0) {X = 0;} else if (X > 1){X = 1;}
            Y = 0.212671 * R/255 + 0.71516 * G/255 + 0.072169 * B/255;
            if (Y < 0) {Y = 0;} else if (Y > 1){Y = 1;}
            Z= 0.019334 * R/255 + 0.119193 * G/255 + 0.950227 * B/255;
            if (Z < 0) {Z = 0;} else if (Z > 1){Z = 1;}
            float h = 255.0;
            X *= h;
            Y *= h;
            Z *= h;
            rgb = qRgba(X, Y, Z, 255);
        }
    }
}

void Transformation::XYZtoRGB (QImage &image) {
#pragma omp parallel for
    for (int y = 0; y < image.height() ; ++y) {
        QRgb *line;
#pragma omp critical
        {
            line = reinterpret_cast<QRgb*>(image.scanLine(y));
        }
        for (int x = 0; x < image.width(); ++x) {
            QRgb &rgb = line[x];
            float R, G, B;
            float X = qRed(rgb);
            float Y = qGreen(rgb);
            float Z = qBlue(rgb);
            R = 3.240479 * X/255 - 1.53715 * Y/255 - 0.498535 * Z/255;
            if (R < 0) {R = 0;} else if (R > 1){R = 1;}
            G = -0.969256 * X/255 + 1.875991 * Y/255 + 0.041556 * Z/255;
            if (G < 0) {G = 0;} else if (G > 1){G = 1;}
            B = 0.055648 * X/255 - 0.204043 * Y/255 + 1.057311 * Z/255;
            if (B < 0) {B = 0;} else if (B > 1){B = 1;}

            float h = 255.0;
            R *= h;
            G *= h;
            B *= h;
            rgb = qRgba(R, G, B, 255);
        }
    }
}

void Transformation::RGBtoYCC (QImage &image) {
#pragma omp parallel for
    for (int y = 0; y < image.height() ; ++y) {
        QRgb *line;
#pragma omp critical
        {
            line = reinterpret_cast<QRgb*>(image.scanLine(y));
        }
        for (int x = 0; x < image.width(); ++x) {
            QRgb &rgb = line[x];

            float Y, C1, C2;
            float R = qRed(rgb) / 255.0f;
            float G = qGreen(rgb) / 255.0f;
            float B = qBlue(rgb) / 255.0f;

            Y = 0.299f * R + 0.587f * G + 0.114f * B;
            C1 = -0.299f * R - 0.587f * G + 0.886f * B;
            C2 = 0.701f * R - 0.587f * G - 0.114f * B;
            Y = 1.0f / 1.402f * Y;
            C1 = 111.4f / 255.0f * C1 + 156.0f / 255.0f;
            C2 = 135.64f / 255.0f * C2 + 137.0f / 255.0f;

            float h = 255.0f;
            Y *= h;
            C1 *= h;
            C2 *= h;

            rgb = qRgba(Y, C1, C2, 255);
        }
    }
}

void Transformation::YCCtoRGB (QImage &image) {
#pragma omp parallel for
    for (int y = 0; y < image.height() ; ++y) {
        QRgb *line;
#pragma omp critical
        {
            line = reinterpret_cast<QRgb*>(image.scanLine(y));
        }
        for (int x = 0; x < image.width(); ++x) {
            QRgb &rgb = line[x];

            float R, G, B;
            float Y = qRed(rgb) / 255.f;
            float C1 = qGreen(rgb) / 255.f;
            float C2 = qBlue(rgb) / 255.f;

            Y = 1.3584f * Y;
            C1 = 2.2179f * (C1 - 156.0f / 255.f);
            C2 = 1.8215f * (C2 - 137.0f / 255.f);

            R = Y + C2;
            G = Y - 0.194f * C1 - 0.509f * C2;
            B = Y + C1;

            float h = 255.f;
            R *= h;
            G *= h;
            B *= h;
            R = std::fmax(0, std::fmin(255, R));
            G = std::fmax(0, std::fmin(255, G));
            B = std::fmax(0, std::fmin(255, B));

            rgb = qRgba(R, G, B, 255);
        }
    }
}

void Transformation::RGBtoHLS (QImage &image) {
#pragma omp parallel for
    for (int y = 0; y < image.height() ; ++y) {
        QRgb *line;
#pragma omp critical
        {
            line = reinterpret_cast<QRgb*>(image.scanLine(y));
        }
        for (int x = 0; x < image.width(); ++x) {
            QRgb &rgb = line[x];

            float H, S;
            float R = qRed(rgb) / 255.0f;
            float G = qGreen(rgb) / 255.0f;
            float B = qBlue(rgb) / 255.0f;
            // Lightness
            float M1 = std::max(std::max(R, G), B);
            float M2 = std::min(std::min(R, G), B);
            float L = (M1 + M2) / 2.0f;
            // Saturation
            if (M1 == M2) {
                S = 0.0f;
                H = 0.0f;
            } else {
                if (L <= 0.5f) {
                    S = (M1 - M2) / (M1 + M2);
                } else {
                    S = (M1 - M2) / (2 - M1 - M2);
                }
            }
            // Hue
            float Cr, Cg, Cb;
            Cr = (M1 - R) / (M1 - M2);
            Cg = (M1 - G) / (M1 - M2);
            Cb = (M1 - B) / (M1 - M2);
            if (R == M1) {H = Cb - Cg;}
            if (G == M1) {H = 2.0f + Cr - Cb;}
            if (B == M1) {H = 4.0f + Cg - Cr;}
            H = 60.0f * H;
            if (H < 0.0f) {H = H + 360.0f;}

            float h = 255.0f;
            H = 255.0f * (H / 360.0f);
            L *= h;
            S *= h;

            rgb = qRgba(H, L, S, 255);
        }
    }

}

void Transformation::HLStoRGB (QImage &image) {
#pragma omp parallel for
    for (int y = 0; y < image.height() ; ++y) {
        QRgb *line;
#pragma omp critical
        {
            line = reinterpret_cast<QRgb*>(image.scanLine(y));
        }
        for (int x = 0; x < image.width(); ++x) {
            QRgb &rgb = line[x];

            float R, G, B, h = 0.0f;
            float H = (qRed(rgb) / 255.0f) * 360.0f;
            float L = qGreen(rgb) / 255.0f;
            float S = (qBlue(rgb) / 255.0f);
            float k = H / 60.f;

            int cel = k / 2;
            k = k - cel * 2;

            float C = (1 - fabs(2 * L - 1)) * S;
            float X = C * (1 - abs((k- 1)));
            float m = L - C / 2;

            if (H < 60) {
                R = C;
                G = X;
                B = 0;
            } else if (H < 120) {
                R = X;
                G = C;
                B = 0;
            } else if (H < 180) {
                R = 0;
                G = C;
                B = X;
            } else if (H < 240) {
                R = 0;
                G = X;
                B = C;
            } else if (H < 300) {
                R = X;
                G = 0;
                B = C;
            } else {
                R = C;
                G = 0;
                B = X;

            }

            h = 255.0f;
            R += m;
            G += m;
            B += m;
            R *= h;
            G *= h;
            B *= h;

            rgb = qRgba(R, G, B, 255);
        }
    }
}

void Transformation::RGBtoGray (QImage &image) {
#pragma omp parallel for
    for (int y = 0; y < image.height() ; ++y) {
        QRgb *line;
#pragma omp critical
        {
            line = reinterpret_cast<QRgb*>(image.scanLine(y));
        }
        for (int x = 0; x < image.width(); ++x) {
            QRgb &rgb = line[x];

            float R = qRed(rgb);
            float G = qGreen(rgb);
            float B = qBlue(rgb);

            float Y = 0.299f * R + 0.587f * G + 0.114f * B;

            rgb = qRgba(Y, Y, Y, 255);
        }
    }
}

void Transformation::MedianFilter (QImage &image) {
#pragma omp parallel for
    for (int y = 0; y < image.height() ; ++y) {
        QRgb *line;
#pragma omp critical
        {
            line = reinterpret_cast<QRgb*>(image.scanLine(y));
        }
        for (int x = 0; x < image.width(); ++x) {
            std::vector<QRgb> mask(5);
            int k = 0;
            for (; k < 5; ++k) {
                mask[k] = line[x + k];
            }
            sort(mask.begin(), mask.end());
            line[x] = mask[2];
        }
    }
}

void Transformation::MedianFilterSquare (QImage &image) {
#pragma omp parallel for
    for (int y = 2; y < image.height() - 2; ++y) {
        QRgb *line;
        for (int x = 2; x < image.width() - 2; ++x) {
            std::vector<QRgb> mask(25);
            for (int j = 0; j < 5; ++j) {
                line = reinterpret_cast<QRgb*>(image.scanLine(y-2 + j));
                for (int k = 0; k < 5; ++k) {
                    mask[k + j * 5] = line[k + x-2];
                }
            }
            sort(mask.begin(), mask.end());
            line = reinterpret_cast<QRgb*>(image.scanLine(y));
            line[x] = mask[12];
        }
    }
}

