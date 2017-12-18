#ifndef MANIPULATIONFUNCS_H
#define MANIPULATIONFUNCS_H
#include <QImage>

void grayscale(QImage *image);
void quantization(QImage *image, int numOfColors, int mostFrequentValues[]);
QImage cutImage(QImage image, QImage targetImage, int xIntersec, int yIntersec);
int countColorsLuminance(QImage image);
void textureSynthesis(QImage *background, QImage grayForeground, QImage cutGrayBg, int x, int y, int numOfColors);
void histogramMatching(QImage *image, QImage target);
void save(QImage *image, char* format);
void mostFrequentColors(QImage *image, int numOfColors, int mostFrequentValues[256]);
void histogramComputation(QImage *image, int histogram[]);
void mergeImages(QImage *image1, QImage image2);
void normalizeHist(int histSrc[256], int histTarget[256]);

#endif // MANIPULATIONFUNCS_H
