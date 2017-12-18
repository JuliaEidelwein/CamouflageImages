#include "manipulationfuncs.h"
#include <time.h>
#include <QRgb>

void grayscale(QImage *image){
    float L, alpha;
    int i, j, imgH, imgW;
    imgH = image->height();
    imgW = image->width();
    for(i = 0; i < imgW; i++){
        for(j = 0; j < imgH; j++){
            L = 0.299*qRed(image->pixel(i,j)) + 0.587*qGreen(image->pixel(i,j)) +  0.114*qBlue(image->pixel(i,j));
            alpha = qAlpha(image->pixel(i,j));
            image->setPixel(QPoint(i,j),qRgba(L,L,L,alpha));

        }
    }
}

void quantization(QImage *image, int numOfColors, int mostFrequentValues[]){
    int diff, closest, shade, i, j, k, imgH, imgW;
    imgH = image->height();
    imgW = image->width();
    for(i = 0; i < imgW; i++){
        for(j = 0; j < imgH; j++){
            if(qAlpha(image->pixel(i,j)) > 0){
                shade = qRed(image->pixel(i,j));
                diff = abs(shade - mostFrequentValues[j]);
                for(k = 0; k < numOfColors; k++){
                    if(abs(shade - mostFrequentValues[k]) < diff){
                        diff = abs(shade - mostFrequentValues[k]);
                        closest = mostFrequentValues[k];
                    }
                }
                image->setPixel(QPoint(i,j),qRgb(closest,closest,closest));
            }
        }
    }
}


QImage cutImage(QImage image, QImage targetImage, int xIntersec, int yIntersec){
    QImage cut(targetImage.width(), targetImage.height(), targetImage.format());
    int i, j;
    for(i = 0; i < targetImage.width(); i++){
        for(j = 0; j < targetImage.height(); j++){
            if(qAlpha(targetImage.pixel(i,j)) == 0){
                cut.setPixel(i, j, qRgba(255, 255, 255, 0));
            } else {
                cut.setPixel(i, j, image.pixel((xIntersec + i), (yIntersec + j)));
            }
        }
    }
    return cut;
}

int countColorsLuminance(QImage image){
    int i, j, colors[256] = {0}, total = 0;
    for(i = 0; i < image.width(); i++){
        for(j = 0; j < image.height(); j++){
            colors[qRed(image.pixel(i,j))] =  colors[qRed(image.pixel(i,j))] + 1;
        }
    }
    for(i = 0; i < 256; i++){
        if(colors[i] > 0){
            total++;
        }
    }
    return total;
}

void separateSegments(QImage image, int segmentMap[], int numOfColors, int colorSubtitle[]){
    int segmentID = 0, i, j, k, counter = 0, alreadyIn = 0;
    for(i = 0; i < image.width(); i++){
        for(j = 0; j < image.height(); j++){
            if(qAlpha(image.pixel(i,j)) > 0){
                for(k = 0; k < numOfColors; k++){
                    if(qRed(image.pixel(i,j)) == colorSubtitle[k]){
                        alreadyIn = 1;
                        segmentID = k;
                    }
                }
                if(alreadyIn == 0){
                    colorSubtitle[counter] = qRed(image.pixel(i,j));
                    segmentMap[j*image.width() + i] = counter;
                    counter++;
                } else {
                    segmentMap[j*image.width() + i] = segmentID;
                }
                alreadyIn = 0;
            }
        }
    }
}

int getSegment(int shade, int colorSegmentTable[], int numOfColors){
    int i = 0;
    while(i < numOfColors){
        if((shade == (colorSegmentTable[i]))){
            return i;
        }
        i++;
    }
    return 0;
}

void countSegment(QImage image, int colorSegmentTable[], int segmentPopulation[], int numOfColors){
    int i, j;
    for(i = 0; i < image.width(); i++){
        for(j = 0; j < image.height(); j++){
            segmentPopulation[getSegment(qRed(image.pixel(i,j)), colorSegmentTable, numOfColors)] ++;
        }
    }
}

void assingSegments(int colorSegmentTable[], int segmentPopulationBG[], int segmentPopulationFG[], int numOfColors){
    int i, j, closestValue, closest = 0, temp;
    closestValue = 10000;
    for(i = 0; i < numOfColors; i++){
        for(j = i; j < numOfColors; j++){
            if(abs(segmentPopulationBG[i] - segmentPopulationFG[j]) < closestValue){
                closest = j;
            }
        }
        temp = colorSegmentTable[i];
        colorSegmentTable[i] = colorSegmentTable[j];
        colorSegmentTable[j] = temp;
    }
}

void mergeImages(QImage *image1, QImage image2){
    int i, j;
    for(i = 0; i < image1->width(); i++){
        for(j = 0; j < image1->height(); j++){
            if(qAlpha(image1->pixel(i,j)) > 0 && qAlpha(image2.pixel(i,j)) > 0){
                image1->setPixel(i, j, (image1->pixel(i,j) + image2.pixel(i,j)));
            }
        }
    }
}


void mergeImages2(QImage *image1, QImage image2){
    int i, j;
    for(i = 0; i < image1->width(); i++){
        for(j = 0; j < image1->height(); j++){
            if(qAlpha(image1->pixel(i,j)) > 0 && qAlpha(image2.pixel(i,j)) > 0){
                image1->setPixel(i, j,(qRgba(qRed(image1->pixel(i,j)),qGreen(image1->pixel(i,j)),qBlue(image1->pixel(i,j)),220) + qRgba(qRed(image2.pixel(i,j)),qGreen(image2.pixel(i,j)),qBlue(image2.pixel(i,j)),35)));
            }
        }
    }
}
void textureSynthesis(QImage *background, QImage grayForeground, QImage cutGrayBg, int x, int y, int numOfColors){
    int i, j, k, segmentMap[grayForeground.height()*grayForeground.width()];
    int colorSegmentTable[numOfColors];
    int segmentPopFG[numOfColors] = {0}, segmentPopBG[numOfColors] = {0};
    int  segment;
    QImage *aux = new QImage(grayForeground.width(), grayForeground.height(), grayForeground.format());
    *aux = grayForeground;
    for(i = 0; i < grayForeground.height()*grayForeground.width(); i++){
        segmentMap[i] = -1;
    }
    separateSegments(cutGrayBg,segmentMap,numOfColors,colorSegmentTable);

    int l = 0, lastL = 0;
    for(k = 0; k < numOfColors; k++){
        for(i = 0; i < grayForeground.width(); i++){
            for(j = 0; j < grayForeground.height(); j++){
                if(qAlpha(grayForeground.pixel(i,j)) > 0){
                    segment = getSegment(qRed(grayForeground.pixel(i,j)), colorSegmentTable, numOfColors);
                    if(segment == k){
                        l = lastL;
                        while(l < grayForeground.height()*grayForeground.width()){
                            if(colorSegmentTable[l] == segment && qAlpha(background->pixel((x + (l%grayForeground.width())), (y + (l/grayForeground.width())))) > 0){
                               aux->setPixel(i,j, (background->pixel((x + (l%grayForeground.width())), (y + (l/grayForeground.width())))));
                               lastL = l;
                               l = grayForeground.height()*grayForeground.width();
                            } else {
                                if(l == grayForeground.height()*grayForeground.width() - 1 && lastL > 0){
                                    l = -1;
                                    lastL = 0;
                                }
                            }
                            l++;
                        }
                    }
                }
            }
        }
    }
    aux->save("auxFilled.png");

    for(i = 0; i < grayForeground.width(); i++){
        for(j = 0; j < grayForeground.height(); j++){
            if(qAlpha(aux->pixel(i,j)) > 0){
                background->setPixel((x + i),(y + j),(aux->pixel(i,j)));
                //background->setPixel((x + i),(y + j),qRgba(qRed(aux->pixel(i,j)),qGreen(aux->pixel(i,j)),qBlue(aux->pixel(i,j)), 100));
            }
        }
    }
}

void mostFrequentColors(QImage *image, int numOfColors,int mostFrequentValues[256]){
    int i, j, max, mindex, histogram[256] = {0};
    histogramComputation(image, histogram);
    for(i = 0; i < numOfColors; i++){
        max = 0;
        mindex = 0;
        for(j = 0; j < 256; j++){
         if(histogram[j] >= max){
             max = histogram[j];
             mindex = j;
         }
        }
        histogram[mindex] = 0;
        mostFrequentValues[i] = mindex;
    }
}

void histogramComputation(QImage *image, int histogram[]){
    int i, j, imgH, imgW;
    imgH = image->height();
    imgW = image->width();

    for(i = 0; i < 256; i++){
        histogram[i] = 0;
    }
    for(i = 0; i < imgW; i++){
        for(j = 0; j < imgH; j++){
           histogram[qRed(image->pixel(i,j))] = histogram[qRed(image->pixel(i,j))] + 1;
        }
    }
}

void normalizeHist(int histSrc[256], int histTarget[256]){
    int i, maxTarget = 0, maxSrc = 0, coef = 0;
    for(i = 0; i < 255; i++){
        if(histSrc[i] >= maxSrc){
            maxSrc = histSrc[i];
        }
        if(histTarget[i] >= maxTarget){
            maxTarget = histTarget[i];
        }
    }
    if(maxTarget > maxSrc){
        if(maxSrc == 0){
            coef = maxTarget;
        } else {
            coef = maxTarget/maxSrc;
        }
        for(i = 0; i < 255; i++){
            histSrc[i] = histSrc[i]*coef;
        }
    } else {
        if(maxTarget == 0){
            coef = maxSrc;
        } else {
            coef = maxSrc/maxTarget;
        }
        for(i = 0; i < 255; i++){
            histTarget[i] = histTarget[i]*coef;
        }
    }
}
