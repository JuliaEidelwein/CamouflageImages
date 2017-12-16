#ifndef MANIPULATIONFUNCS_H
#define MANIPULATIONFUNCS_H
#include <QImage>

void verticalMirroring(QImage *image);

QImage verticalMirroring2(QImage image);

void horizontalMirroring(QImage *image);
void grayscale(QImage *image);
void quantization(QImage *image, int numOfColors, int mostFrequentValues[]);
//void quantization(QImage *image, int numOfColors);
QImage cutImage(QImage image, QImage targetImage, int xIntersec, int yIntersec);
//QImage copyPartOf(QImage image, QImage copy, int x, int y, int w, int h);
int countColorsLuminance(QImage image);
void textureSynthesis(QImage *background, QImage grayForeground, QImage cutGrayBg, int x, int y, int numOfColors);
void histogramMatching(QImage *image, QImage target);
void save(QImage *image, char* format);
void mostFrequentColors(QImage *image, int numOfColors, int mostFrequentValues[256]);
void histogramComputation(QImage *image, int histogram[]);
void normalizeRGB(int *r, int *g, int *b);
void brightness(QImage *image, int bright);
void contrast(QImage *image, float contr);
void negative(QImage *image);
void equalization(QImage *image);
void mergeImages(QImage *image1, QImage image2);
//void histogramMatching(QImage *image, QImage *target);
QImage zoomOut(QImage image, int zoomV, int zoomH);
QImage zoomIn(QImage image);
void normalizeHist(int histSrc[256], int histTarget[256]);
QImage rotateRight(QImage image);
QImage rotateLeft(QImage image);
QImage applyFilter(QImage image, double kernel[9], int filter);

#endif // MANIPULATIONFUNCS_H
