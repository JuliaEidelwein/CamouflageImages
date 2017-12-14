#include "manipulationfuncs.h"
#include <QRgb>

void verticalMirroring(QImage *image){
    uchar *aux1, *aux2, *aux3;
    int i, j, imgH, imgW;
    imgH = image->height();
    imgW = image->width();
    aux3 = (uchar*) malloc(sizeof(QRgb)*imgW);
    for(i = 0, j = imgH-1; i < floor(imgH/2); i++, j--){
        aux1 = image->scanLine(i);
        aux2 = image->scanLine(j);
        memcpy(aux3,aux1,sizeof(QRgb)*imgW);
        memcpy(aux1,aux2,sizeof(QRgb)*imgW);
        memcpy(aux2,aux3,sizeof(QRgb)*imgW);
    }
    free(aux3);
    aux3 = NULL;
}

void horizontalMirroring(QImage *image){
    uchar *aux1 = NULL, *aux2;
    int i, j, imgH, imgW;
    imgH = image->height();
    imgW = image->width();
    aux1 = (uchar*) malloc(sizeof(QRgb));
    for(i = 0; i < imgH; i++){
        aux2 = image->scanLine(i);
        for(j = 0; j < floor(imgW/2); j++){
             memcpy(aux1,(aux2 + j*sizeof(QRgb)),sizeof(QRgb));
             memcpy((aux2 + j*sizeof(QRgb)),(aux2 + (imgW - j - 1)*sizeof(QRgb)),sizeof(QRgb));
             memcpy((aux2 + (imgW - j - 1)*sizeof(QRgb)),aux1,sizeof(QRgb));
        }
    }
    free(aux1);
    aux1 = NULL;
}

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

/*void quantization(QImage *image, int numOfColors, int mostFrequentValues[]){
    int diff, closest, shade, i, j, k, imgH, imgW;
    imgH = image->height();
    imgW = image->width();
    for(i = 0; i < imgW; i++){
        for(j = 0; j < imgH; j++){
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
}*/

void quantization(QImage *image, int numOfColors){
    int shade, i, j, k, imgH, imgW, increment, alpha;
    int qShades[numOfColors];
    imgH = image->height();
    imgW = image->width();
    if(numOfColors < 2){
        numOfColors = 2;
    }
    increment = 255/(numOfColors - 1);
    for(i = 0; i < numOfColors; i++){
        qShades[i] = floor(i*increment);
    }
    for(i = 0; i < imgW; i++){
        for(j = 0; j < imgH; j++){
            shade = qRed(image->pixel(i,j));
            alpha = qAlpha(image->pixel(i,j));
            if(shade > (qShades[numOfColors - 1]/2)){
                shade = qShades[numOfColors - 1];
            } else {
                for(k = 0; k < numOfColors - 1; k++){
                    if(shade > qShades[k] && shade < (qShades[k+1]/2)){
                        shade = qShades[k];
                    }
                }
            }
            image->setPixel(QPoint(i,j),qRgba(shade,shade,shade,alpha));
        }
    }
     /*       image->setPixel(QPoint(i,j),qRgb(shade,shade,shade,alpha));
            diff = abs(shade - shades[j]);
            for(k = 0; k < numOfColors; k++){
                if(abs(shade - shades[k]) < diff){
                    diff = abs(shade - mostFrequentValues[k]);
                    closest = mostFrequentValues[k];
                }
            }
            image->setPixel(QPoint(i,j),qRgb(closest,closest,closest));
        }
    }*/
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


void normalizeRGB(int *r, int *g, int *b){
    if(*r > 255){
        *r = 255;
    } else {
        if(*r < 0){
            *r = 0;
        }
    }
    if(*g > 255){
        *g = 255;
    } else {
        if(*g < 0){
            *g = 0;
        }
    }
    if(*b > 255){
        *b = 255;
    } else {
        if(*b < 0){
            *b = 0;
        }
    }
}

void brightness(QImage *image, int bright){
    int i, j, imgH, imgW, shadeR, shadeG, shadeB;
    imgH = image->height();
    imgW = image->width();

    for(i = 0; i < imgW; i++){
        for(j = 0; j < imgH; j++){
            shadeR = qRed(image->pixel(i,j)) + bright;
            shadeG = qGreen(image->pixel(i,j)) + bright;
            shadeB = qBlue(image->pixel(i,j)) + bright;
            normalizeRGB(&shadeR,&shadeG,&shadeB);
            image->setPixel(QPoint(i,j),qRgb(shadeR,shadeG,shadeB));
        }
    }
}

void contrast(QImage *image, float contr){
    int i, j, imgH, imgW, shadeR, shadeG, shadeB;
    imgH = image->height();
    imgW = image->width();

    for(i = 0; i < imgW; i++){
        for(j = 0; j < imgH; j++){
            shadeR = (int)round(qRed(image->pixel(i,j))*contr);
            shadeG = (int)round(qGreen(image->pixel(i,j))*contr);
            shadeB = (int)round(qBlue(image->pixel(i,j))*contr);
            normalizeRGB(&shadeR,&shadeG,&shadeB);
            image->setPixel(QPoint(i,j),qRgb(shadeR,shadeG,shadeB));
        }
    }
}

void negative(QImage *image){
    int i, j, imgH, imgW, shadeR, shadeG, shadeB;
    imgH = image->height();
    imgW = image->width();

    for(i = 0; i < imgW; i++){
        for(j = 0; j < imgH; j++){
            shadeR = 255 - qRed(image->pixel(i,j));
            shadeG = 255 - qGreen(image->pixel(i,j));
            shadeB = 255 - qBlue(image->pixel(i,j));
            image->setPixel(QPoint(i,j),qRgb(shadeR,shadeG,shadeB));
        }
    }
}

void equalization(QImage *image){
    int histogram[256] = {0}, cumulative[256] = {0}, i, j;
    int cumulativeR, cumulativeG, cumulativeB;
    int size = image->width()*image->height();
    float alpha = (float)255/size;
    QImage gray = *image;
    grayscale(&gray);
    histogramComputation(&gray,histogram);
    cumulative[0] = alpha*histogram[0];
    for(i = 1; i < 256; i++){
        cumulative[i] = cumulative[i-1] + alpha*histogram[i];
    }
    for(i=0; i < image->width() ; i++){
        for(j=0; j < image->height() ; j++){
            cumulativeR = cumulative[(qRed(image->pixel(i,j)))];
            cumulativeG = cumulative[(qGreen(image->pixel(i,j)))];
            cumulativeB = cumulative[(qBlue(image->pixel(i,j)))];
            image->setPixel(i,j,qRgb(cumulativeR, cumulativeG, cumulativeB));
        }
    }
}

void histogramMatching(QImage *image, QImage *target){
    int histSrc[256], histTarget[256], histSrcCumulative[256], histTargetCumulative[256], normalizedSrc[256], normalizedTarget[256];
    int HM[256], shade, i, j, diff, closestShade, used[256] = {0};
    int size = image->width()*image->height();
    float alpha = (float)255/size;
    histogramComputation(image,histSrc);
    histogramComputation(target,histTarget);
    histSrcCumulative[0] = alpha*histSrc[0];
    histTargetCumulative[0] = alpha*histTarget[0];
    normalizedSrc[0] = histSrcCumulative[0];
    normalizedTarget[0] = histTargetCumulative[0];
    for(i = 1; i < 256; i++){
        histSrcCumulative[i] = histSrcCumulative[i-1] + alpha*histSrc[i];
        histTargetCumulative[i] = histTargetCumulative[i-1] + alpha*histTarget[i];
        normalizedSrc[i] = histSrcCumulative[i];
        normalizedTarget[i] = histTargetCumulative[i];
    }
    normalizeHist(normalizedSrc,normalizedTarget);
    for(shade = 0; shade < 256; shade++){
        diff = size+10;
        for(i = 0; i < 256; i++){
            if(abs(normalizedTarget[i] - normalizedSrc[shade]) < diff && used[i] == 0){
                diff = abs(normalizedTarget[i] - normalizedSrc[shade]);
                closestShade = i;
            }
        }
        HM[shade] = closestShade;
        used[closestShade] = 1;
    }

    for(i=0; i < image->width() ; i++){
        for(j=0; j < image->height() ; j++){
            shade = HM[qRed(image->pixel(i,j))];
            image->setPixel(i,j,qRgb(shade,shade,shade));
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

QImage zoomOut(QImage image, int zoomV, int zoomH){
    int i, j, k, l, red = 0, green = 0, blue = 0, width, height, size, restV, restH, height2, width2, sizeH, sizeV;
    size = zoomV*zoomH;
    height = (int)ceil((double)image.height()/zoomV);
    width = (int)ceil((double)image.width()/zoomH);
    height2 = height;
    width2 = width;
    if(image.height() % zoomV != 0){
        height2 = height - 1;
    }
    if(image.width() % zoomH != 0){
        width2 = width - 1;
    }
    QImage resized = QImage(width,height,image.format());

    for(i = 0; i < height2; i++){
        for(j = 0; j < width2; j++){
            red = 0;
            green = 0;
            blue = 0;
            for(k = 0; k < zoomV; k++){
                for(l = 0; l < zoomH; l++){
                    red = red + qRed(image.pixel((j*zoomH + l),(i*zoomV + k)));
                    green = green + qGreen(image.pixel((j*zoomH + l),(i*zoomV + k)));
                    blue = blue + qBlue(image.pixel((j*zoomH + l),(i*zoomV + k)));
                }
            }
            resized.setPixel(j,i,qRgb(red/size,green/size,blue/size));
        }
    }
    restV = zoomV;
    restH = zoomH;
    if(image.height() % zoomV != 0){
        restV = (image.height() - height2*zoomV);
    }
    if(image.width() % zoomH != 0){
        restH = (image.width() - width2*zoomH);
    }
    sizeV = restV*zoomH;
    sizeH = restH*zoomV;
    if(image.width() % zoomH != 0){
        for(i = 0; i < height; i++){
            for(j = (width2*zoomH); j <= (image.width() - restH); j++){
                red = 0;
                green = 0;
                blue = 0;
                for(k = 0; k < restV; k++){
                    for(l = 0; l < restH; l++){
                        red = red + qRed(image.pixel((j + l),(i*zoomV + k)));
                        green = green + qGreen(image.pixel((j + l),(i*zoomV + k)));
                        blue = blue + qBlue(image.pixel((j + l),(i*zoomV + k)));
                    }
                }
            }
            resized.setPixel(width - 1,i,qRgb(red/sizeH,green/sizeH,blue/sizeH));
        }
    }
    if(image.height() % zoomV != 0){
        for(j = 0; j < width; j++){
            for(i = (height2*zoomV); i <= (image.height() - restV); i++){
                red = 0;
                green = 0;
                blue = 0;
                for(l = 0; l < restH; l++){
                    for(k = 0; k < restV; k++){
                        red = red + qRed(image.pixel((j*zoomH + l),(i + k)));
                        green = green + qGreen(image.pixel((j*zoomH + l),(i + k)));
                        blue = blue + qBlue(image.pixel((j*zoomH + l),(i + k)));
                    }
                }
            }
            resized.setPixel(j,height - 1,qRgb(red/sizeV,green/sizeV,blue/sizeV));
        }
    }
    return resized;
}

QImage zoomIn(QImage image){
    int i, j, red = 0, green = 0, blue = 0, width, height;
    height = image.height()*2 - 1;
    width = image.width()*2 - 1;
    QImage resized = QImage(width,height,image.format());

    for(i = 0; i < height; i++){
        for(j = 0; j < width; j++){
            if(i % 2 == 0 && j % 2 == 0){
                red  = qRed(image.pixel(j/2,i/2));
                green = qGreen(image.pixel(j/2,i/2));
                blue = qBlue(image.pixel(j/2,i/2));
                resized.setPixel(j,i,qRgb(red,green,blue));
            } else {
                resized.setPixel(j,i,qRgb(255,255,255));
            }
        }
    }
    for(i = 0; i < height; i = i + 2){
        for(j = 0; j < width - 1; j = j + 2){
            if(i % 2 == 0){
                red  = ceil((double)(qRed(resized.pixel(j,i)) + qRed(resized.pixel(j + 2,i)))/2);
                green = ceil((double)(qGreen(resized.pixel(j,i)) + qGreen(resized.pixel(j + 2,i)))/2);
                blue = ceil((double)(qBlue(resized.pixel(j,i)) + qBlue(resized.pixel(j + 2,i)))/2);
                resized.setPixel(j+1,i,qRgb(red,green,blue));
            }
        }
    }
    for(j = 0; j < width; j++){
        for(i = 1; i < height - 1; i = i + 2){
            red  = ceil((double)(qRed(resized.pixel(j,i - 1)) + qRed(resized.pixel(j,i + 1)))/2);
            green = ceil((double)(qGreen(resized.pixel(j,i - 1)) + qGreen(resized.pixel(j,i + 1)))/2);
            blue = ceil((double)(qBlue(resized.pixel(j,i - 1)) + qBlue(resized.pixel(j,i + 1)))/2);
            resized.setPixel(j,i,qRgb(red,green,blue));
        }
    }
    return resized;
}

QImage rotateRight(QImage image){
    QImage rotated = QImage(image.height(),image.width(),image.format());
    int i, j, red, green, blue;
    for(i = 0; i < image.height(); i++){
        for(j = 0; j < image.width(); j++){
            red  = qRed(image.pixel(j,(image.height() - (i + 1))));
            green = qGreen(image.pixel(j,(image.height() - (i + 1))));
            blue = qBlue(image.pixel(j,(image.height() - (i + 1))));
            rotated.setPixel(i,j,qRgb(red,green,blue));
        }
    }
    return rotated;
}

QImage rotateLeft(QImage image){
    QImage rotated = QImage(image.height(),image.width(),image.format());
    int i, j, red, green, blue;
    for(i = 0; i < image.height(); i++){
        for(j = 0; j < image.width(); j++){
            red  = qRed(image.pixel((image.width() - (j + 1)),i));
            green = qGreen(image.pixel((image.width() - (j + 1)),i));
            blue = qBlue(image.pixel((image.width() - (j + 1)),i));
            rotated.setPixel(i,j,qRgb(red,green,blue));
        }
    }
    return rotated;
}

QImage applyFilter(QImage image, double kernel[9], int filter){
    QImage aux = QImage(image.width(),image.height(),image.format());
    int i, j;
    int convolution;
    aux = image;
    for(i = 1; i < (image.height() - 1); i++){
        for(j = 1; j < (image.width() - 1); j++){
            convolution = (int)(qRed(aux.pixel(j - 1, i - 1))*kernel[8] + qRed(aux.pixel(j, i - 1))*kernel[7]
                    + qRed(aux.pixel(j + 1, i - 1))*kernel[6] + qRed(aux.pixel(j - 1, i))*kernel[5]
                    + qRed(aux.pixel(j, i))*kernel[4] + qRed(aux.pixel(j + 1, i))*kernel[3]
                    + qRed(aux.pixel(j - 1, i + 1))*kernel[2] + qRed(aux.pixel(j, i + 1))*kernel[1]
                    + qRed(aux.pixel(j + 1, i + 1))*kernel[0]);
            if(filter > 3){
                convolution = convolution + 127;
            }
            normalizeRGB(&convolution, &convolution, &convolution);
            image.setPixel(j,i,qRgb(convolution, convolution, convolution));
        }
    }
    return image;
}
