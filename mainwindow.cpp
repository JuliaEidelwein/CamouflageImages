#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "manipulationfuncs.h"
#include <QImage>
#include <QString>
#include <QDir>
#include <QFileDialog>
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QImage *img = new QImage("://resizing.png");
    ui->setupUi(this);

    ui->srEdge->setPixmap(QPixmap::fromImage(img->scaled(15, 15, Qt::IgnoreAspectRatio)));
    ui->srEdge->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
    ui->srEdge->hide();
    ui->noScale->hide();
    ui->foreground->hide();
    setAcceptDrops(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked(){
    int labelH, labelW;
    QString filename =  QFileDialog::getOpenFileName(this,"Open Document",QDir::currentPath(),"Images (*.png *.xpm *.jpg *.jpeg);;All files (*.*) ;; Document files (*.doc *.rtf);; PNG files (*.png)");
    QImage img, showed_img;
    loaded = 0;
    if( !filename.isNull() ){
        img.load(filename);
        this->loaded++;
    }
    labelH = ui->background->height();
    labelW = ui->background->width();

    showed_img = img.scaled(labelW,labelH,Qt::KeepAspectRatio);
    ui->background->setPixmap(QPixmap::fromImage(showed_img));
    dropPosition = QPoint(ui->background->x(), ui->background->y());
}

void MainWindow::on_pushButton_2_clicked(){
    int labelH, labelW;
    if(loaded > 0){
        QString filename =  QFileDialog::getOpenFileName(this,"Open Document",QDir::currentPath(),"Images (*.png *.xpm *.jpg *.jpeg);;All files (*.*) ;; Document files (*.doc *.rtf);; PNG files (*.png)");
        QImage img, showed_img;
        loaded2 = 0;
        if( !filename.isNull() ){
            img.load(filename);
            this->loaded2++;
        }
        ui->noScale->setPixmap(QPixmap::fromImage(img));
        ui->noScale->setVisible(false);
        labelH = ui->background->height()/2;
        labelW = ui->background->width()/2;
        showed_img = img.scaled(labelW,labelH,Qt::KeepAspectRatio);
        ui->foreground->setGeometry(70,70,labelW,labelH);
        ui->foreground->setPixmap(QPixmap::fromImage(showed_img));
        labelH = ui->foreground->pixmap()->height();
        labelW = ui->foreground->pixmap()->width();
        ui->foreground->setGeometry(70,70,labelW,labelH);
        ui->foreground->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
        ui->foreground->show();
        ui->foreground->raise();
        ui->frame->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
        ui->frame->raise();
    }
}

void MainWindow::on_pushButton_3_clicked(){
    QImage img;
    if(loaded2 > 0){
        img = (ui->foreground->pixmap())->toImage();
        ui->foreground->setPixmap(QPixmap::fromImage(img.mirrored(true,false)));
    }
}

void MainWindow::on_pushButton_4_clicked(){
    QImage img;
    if(loaded2 > 0){
        img = (ui->foreground->pixmap())->toImage();
        ui->foreground->setPixmap(QPixmap::fromImage(img.mirrored(false,true)));
    }
}

void MainWindow::on_pushButton_5_clicked(){
    QImage img, showed_img;
    int imgX, imgY, imgH, imgW;
    if(loaded2 > 0){
            resizing = 1;
            imgX = ui->foreground->x();
            imgY = ui->foreground->y();
            imgH = ui->foreground->pixmap()->height();
            imgW = ui->foreground->pixmap()->width();
            ui->srEdge->show();
            ui->srEdge->raise();
            ui->srEdge->setGeometry((imgX + imgW - 5), (imgY - 5), 15, 15);
            imgX = ui->foreground->x();
            imgY = ui->foreground->y();
            imgH = ui->foreground->pixmap()->height();
            imgW = ui->foreground->pixmap()->width();
            ui->srEdge->raise();
            ui->srEdge->setGeometry((imgX + imgW - 5), (imgY - 5), 15, 15);
    }
}

void MainWindow::resizeEvent(QResizeEvent* ev){
   QMainWindow::resizeEvent(ev);
   int imgX, imgY, imgH, imgW;
   if(loaded2 > 0){
       imgX = ui->foreground->x();
       imgY = ui->foreground->y();
       imgH = ui->foreground->pixmap()->height();
       imgW = ui->foreground->pixmap()->width();
       ui->srEdge->raise();
       ui->srEdge->setGeometry((imgX + imgW - 5), (imgY - 5), 15, 15);
   }
}


void MainWindow::mousePressEvent(QMouseEvent *event){
    int height, width, posX, posY;
    if(loaded2 > 0){
        height = ui->foreground->pixmap()->height();
        width = ui->foreground->pixmap()->width();
        posY = ui->foreground->y() + 60;// + ((ui->foreground->height()-height)/2);
        posX = ui->foreground->x() + 24;// + ((ui->foreground->width()-width)/2);
        if (event->button() == Qt::LeftButton && (event->pos().x() >= posX) && (event->pos().x() <= (posX + width))
            && (event->pos().y() >= posY) && (event->pos().y() <= (posY + height)) && (resizing == 0)) {
            dragStartPosition = event->pos();
            canDrag = 1;

            QDrag *drag = new QDrag(this);
            QMimeData *mimeData = new QMimeData;
            drag->setMimeData(mimeData);
            drag->setPixmap(*(ui->foreground->pixmap()));

            ui->foreground->hide();
            Qt::DropAction dropAction = drag->exec();

        } else {
            if(resizing == 1){
                height = ui->srEdge->height();
                width = ui->srEdge->width();
                posY = ui->srEdge->y();
                posX = ui->srEdge->x();
                if (event->button() == Qt::LeftButton && (event->pos().x() >= posX) && (event->pos().x() <= (posX + width))
                    && (event->pos().y() >= posY) && (event->pos().y() <= (posY + height))) {
                    dragStartPosition = event->pos();
                    canDrag = 2;
                    ui->srEdge->hide();

                    QDrag *drag = new QDrag(this);
                    QMimeData *mimeData = new QMimeData;
                    drag->setMimeData(mimeData);
                    Qt::DropAction dropAction = drag->exec();
                }
            } else {
                canDrag = 0;
            }
        }
    }
}


void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(canDrag == 1){
        if (!(event->buttons() & Qt::LeftButton))
            return;
        if ((event->pos() - dragStartPosition).manhattanLength()
             < QApplication::startDragDistance())
            return;

        QDrag *drag = new QDrag(this);
        QMimeData *mimeData = new QMimeData;

        mimeData->setImageData((ui->foreground->pixmap())->toImage());
        drag->setMimeData(mimeData);

        Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction);
    }
}

void MainWindow::dragMoveEvent(QDragMoveEvent *event)
{
    QImage img, showed_img;
    int imgX, imgY, imgH, imgW;
    if (event->answerRect().intersects(ui->background->geometry())){

        if(canDrag == 2){
            imgX = ui->foreground->x();
            imgY = ui->foreground->y();
            if((event->pos().x() - imgX) > 30 && (event->pos().y() - imgY) > 30){
                img = (ui->noScale->pixmap())->toImage();
                ui->foreground->resize((event->pos().x() - imgX),(event->pos().y() - imgY));
                showed_img = img.scaled((event->pos().x() - imgX),(event->pos().y() - imgY),Qt::IgnoreAspectRatio);

                ui->foreground->setPixmap(QPixmap::fromImage(showed_img));

                imgX = ui->foreground->x();
                imgY = ui->foreground->y();
                imgH = ui->foreground->pixmap()->height();
                imgW = ui->foreground->pixmap()->width();
                ui->srEdge->raise();
                ui->srEdge->setGeometry(((event->pos().x() - imgX) - 5), (imgY - 5), 15, 15);
                event->acceptProposedAction();
            }
        } else {
            if(canDrag == 1){
                event->acceptProposedAction();
            }
        }
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event){
        event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event){
    int height, width, posX, posY;
    if(resizing == 0){
        posX = event->pos().x();
        posY = event->pos().y();
        height = ui->foreground->pixmap()->height();
        width = ui->foreground->pixmap()->width();
        //ui->foreground->show();
        ui->foreground->setGeometry(posX - 15, posY - 51, width, height);
        ui->foreground->show();
        dropPosition = QPoint(posX, posY);
    } else {
        resizing = 0;
        ui->srEdge->hide();
    }
    if(canDrag < 3){
        ui->pushButton_6->setEnabled(true);
        event->accept();
    }
}

void MainWindow::on_pushButton_6_clicked()
{   int nColorsForeground, nColorsCut, nColors;
    qInfo("clicou");
    if(this->loaded2 > 0){
        int xIntersec, yIntersec;
        QImage backgroundSeg(ui->background->pixmap()->width(), ui->background->pixmap()->height(), ui->background->pixmap()->toImage().format());
        QImage foregroundSeg(ui->foreground->pixmap()->width(), ui->foreground->pixmap()->height(), ui->foreground->pixmap()->toImage().format());

        backgroundSeg = (ui->background->pixmap()->toImage());
        foregroundSeg = (ui->foreground->pixmap()->toImage());
        ui->foreground->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
        grayscale(&backgroundSeg);
        grayscale(&foregroundSeg);
        //quantization(&backgroundSeg, NCOLORSQUATIZATION);
        //quantization(&foregroundSeg, NCOLORSQUATIZATION);
       /* nColorsCut = countColorsLuminance(cut);
        nColorsForeground = countColorsLuminance(ui->foreground->pixmap()->toImage());
        if(nColorsCut < nColorsForeground){
            quantization(&backgroundSeg, nColorsCut);
            quantization(&foregroundSeg, nColorsCut);
        } else {
            quantization(&backgroundSeg, nColorsForeground);
            quantization(&foregroundSeg, nColorsForeground);
        }*/

        /*xIntersec = ui->foreground->x() - ui->background->x();
        yIntersec = ui->foreground->y() - ui->background->y();*/

       // xIntersec = (ui->foreground->x() + ((ui->foreground->width() - ui->foreground->pixmap()->width())/2)) - (ui->background->x() + ((ui->background->width() - ui->background->pixmap()->width())/2));
       // yIntersec = (ui->foreground->y() + ((ui->foreground->height() - ui->foreground->pixmap()->height())/2)) - (ui->background->y() + ((ui->background->height() - ui->background->pixmap()->height())/2));

        xIntersec = dropPosition.x() - (ui->background->x() + ((ui->background->width() - ui->background->pixmap()->width())/2));
        yIntersec = dropPosition.y() - (ui->background->y() + ((ui->background->height() - ui->background->pixmap()->height())/2)) + 15;

        qInfo("posicao background: %d %d %d %d",(ui->background->x() + ((ui->background->width() - ui->background->pixmap()->width())/2)), (ui->background->y() + ((ui->background->height() - ui->background->pixmap()->height())/2)), ui->background->x(),ui->background->y());
        qInfo("posicao foreground: %d %d %d %d",(ui->foreground->x() + ((ui->foreground->width() - ui->foreground->pixmap()->width())/2)), (ui->foreground->y() + ((ui->foreground->height() - ui->foreground->pixmap()->height())/2)), ui->foreground->x(),ui->foreground->y());
        qInfo("dropPosition: %d %d", dropPosition.x(), dropPosition.y());
        qInfo("Intersec: %d %d", xIntersec, yIntersec);

        QImage cut(ui->foreground->pixmap()->width(), ui->foreground->pixmap()->height(), ui->foreground->pixmap()->toImage().format());
        cut = cutImage(backgroundSeg, foregroundSeg, xIntersec, yIntersec);

        nColorsCut = countColorsLuminance(cut);
        nColorsForeground = countColorsLuminance(ui->foreground->pixmap()->toImage());
        /*if(nColorsCut < nColorsForeground){
            quantization(&backgroundSeg, nColorsCut);
            quantization(&foregroundSeg, nColorsCut);
            nColors = nColorsCut;
        } else {
            quantization(&backgroundSeg, nColorsForeground);
            quantization(&foregroundSeg, nColorsForeground);
            nColors = nColorsForeground;
        }*/
        quantization(&backgroundSeg, 10);
        quantization(&foregroundSeg, 10);
        nColors = 10;

        //cut = cutImage(backgroundSeg, foregroundSeg, xIntersec, yIntersec);

        cut.save("cutResult.png");
        /*if(nColorsCut = countColorsLuminance(cut) < NCOLORSQUATIZATION
        || nColorsForeground = countColorsLuminance(ui->foreground->pixmap()->toImage()) < NCOLORSQUATIZATION){
            if(nColorsCut < nColorsForeground){
                quantization(&backgroundSeg, NCOLORSQUATIZATION);
                quantization(&foregroundSeg, NCOLORSQUATIZATION);
            }
        }*/
        QImage backgroundAux(ui->background->pixmap()->width(), ui->background->pixmap()->height(), ui->background->pixmap()->toImage().format());
        backgroundAux = ui->background->pixmap()->toImage();
        qInfo("Vai entrar na sintese");
        textureSynthesis(&backgroundAux,foregroundSeg, cut, xIntersec, yIntersec, nColors);
        qInfo("colors: %d | %d | %d", nColors, nColorsCut, nColorsForeground);
        backgroundSeg.save("foregroundResult.png");
        foregroundSeg.save("backgroundResult.png");
        backgroundAux.save("resultadoFinal.png");
    }
}
