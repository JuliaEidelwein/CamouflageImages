#include "mainwindow.h"
#include "ui_mainwindow.h"
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
    //ui->srEdge->setVisible(false);
    //ui->slEdge->setVisible(false);
    //ui->irEdge->setVisible(false);
    //ui->ilEdge->setVisible(false);

    ui->srEdge->setPixmap(QPixmap::fromImage(img->scaled(15, 15, Qt::IgnoreAspectRatio)));
    ui->srEdge->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
    ui->srEdge->hide();
    ui->slEdge->hide();
    ui->irEdge->hide();
    ui->ilEdge->hide();
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
    //ui->noScale->setPixmap(QPixmap::fromImage(img));
    //ui->noScale->setVisible(false);
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
        ui->foreground->setGeometry(50,50,labelW,labelH);
        ui->foreground->setPixmap(QPixmap::fromImage(showed_img));
        ui->foreground->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
        ui->foreground->show();
        ui->foreground->raise();
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
        if(resizing == 0){
            resizing = 1;
            imgX = ui->foreground->x();
            imgY = ui->foreground->y();
            //imgH = ui->foreground->height();
            imgH = ui->foreground->pixmap()->height();
            //imgW = ui->foreground->width();
            imgW = ui->foreground->pixmap()->width();
            beforeHeight = ui->foreground->height();
            beforeWidth = ui->foreground->width();
            //ui->srEdge->setGeometry((imgX + imgW - 5), (imgY - 5), 10, 10);
            //ui->slEdge->setGeometry((imgX - 5), (imgY - 5), 10, 10);
            //ui->irEdge->setGeometry((imgX + imgW - 5), (imgY + imgH - 5), 10, 10);
            //ui->ilEdge->setGeometry((imgX - 5), (imgY + imgH - 5), 10, 10);
            ui->srEdge->show();
            ui->slEdge->show();
            ui->irEdge->show();
            ui->ilEdge->show();
            ui->srEdge->raise();
            ui->slEdge->raise();
            ui->irEdge->raise();
            ui->ilEdge->raise();
            ui->srEdge->setGeometry((imgX + imgW - 5), (imgY - 5), 15, 15);
            ui->slEdge->setGeometry((imgX - 5), (imgY - 5), 10, 10);
            ui->irEdge->setGeometry((imgX + imgW - 5), (imgY + imgH - 5), 10, 10);
            ui->ilEdge->setGeometry((imgX - 5), (imgY + imgH - 5), 10, 10);
            imgX = ui->foreground->x();
            imgY = ui->foreground->y();
            //imgH = ui->foreground->height();
            imgH = ui->foreground->pixmap()->height();
            //imgW = ui->foreground->width();
            imgW = ui->foreground->pixmap()->width();
            ui->srEdge->raise();
            ui->slEdge->raise();
            ui->irEdge->raise();
            ui->ilEdge->raise();
            ui->srEdge->setGeometry((imgX + imgW - 5), (imgY - 5), 15, 15);
            ui->slEdge->setGeometry((imgX - 5), (imgY - 5), 10, 10);
            ui->irEdge->setGeometry((imgX + imgW - 5), (imgY + imgH - 5), 10, 10);
            ui->ilEdge->setGeometry((imgX - 5), (imgY + imgH - 5), 10, 10);
            //img = (ui->foreground->pixmap())->toImage();
            //ui->foreground->resize(100, 50);
            //labelH = ui->foreground->height();
            //labelW = ui->foreground->width();
            //showed_img = img.scaled(labelW,labelH,Qt::KeepAspectRatio);
            //ui->foreground->setPixmap(QPixmap::fromImage(showed_img));
        } else {
            resizing = 0;
            ui->srEdge->hide();
            ui->slEdge->hide();
            ui->irEdge->hide();
            ui->ilEdge->hide();
        }
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
       ui->slEdge->raise();
       ui->irEdge->raise();
       ui->ilEdge->raise();
       ui->srEdge->setGeometry((imgX + imgW - 5), (imgY - 5), 15, 15);
       ui->slEdge->setGeometry((imgX - 5), (imgY - 5), 10, 10);
       ui->irEdge->setGeometry((imgX + imgW - 5), (imgY + imgH - 5), 10, 10);
       ui->ilEdge->setGeometry((imgX - 5), (imgY + imgH - 5), 10, 10);
   }
}


void MainWindow::mousePressEvent(QMouseEvent *event){
    //setAcceptDrops(true);
    int height, width, posX, posY;
    if(loaded2 > 0){
       // height = ui->foreground->height();
        height = ui->foreground->pixmap()->height();
        width = ui->foreground->pixmap()->width();
        posY = ui->foreground->y();
        posX = ui->foreground->x();

       // if (event->button() == Qt::LeftButton
            //&& ui->foreground->geometry().contains(event->pos())) {
        if (event->button() == Qt::LeftButton && (event->pos().x() >= posX) && (event->pos().x() <= (posX + width))
            && (event->pos().y() >= posY) && (event->pos().y() <= (posY + height)) && (resizing == 0)) {
            dragStartPosition = event->pos();
            canDrag = 1;
            qInfo("can drag: %d %d %d %d | %d %d", posX, posY, width+posX, height+posY, event->pos().x(), event->pos().y());

            QDrag *drag = new QDrag(this);
            QMimeData *mimeData = new QMimeData;

            //mimeData->setText(commentEdit->toPlainText());
            drag->setMimeData(mimeData);
            drag->setPixmap(*(ui->foreground->pixmap()));

            ui->foreground->hide();
            //this->repaint();

            Qt::DropAction dropAction = drag->exec();

        } else {
            if(resizing == 1){
                qInfo("aqui");
                height = ui->srEdge->height();
                width = ui->srEdge->width();
                posY = ui->srEdge->y();
                posX = ui->srEdge->x();
                qInfo("aqui2");
                if (event->button() == Qt::LeftButton && (event->pos().x() >= posX) && (event->pos().x() <= (posX + width))
                    && (event->pos().y() >= posY) && (event->pos().y() <= (posY + height))) {
                    qInfo("aqui3");
                    dragStartPosition = event->pos();
                    canDrag = 2;
                    ui->srEdge->hide();

                    QDrag *drag = new QDrag(this);
                    QMimeData *mimeData = new QMimeData;
                    qInfo("aqui4");
                    drag->setMimeData(mimeData);
                    qInfo("aqui5");
                    //drag->setPixmap(*(ui->srEdge->pixmap()));
                    qInfo("aqui6");
                    Qt::DropAction dropAction = drag->exec();
                    qInfo("aqui7");
                } else {
                    height = ui->slEdge->height();
                    width = ui->slEdge->width();
                    posY = ui->slEdge->y();
                    posX = ui->slEdge->x();
                    if (event->button() == Qt::LeftButton && (event->pos().x() >= posX) && (event->pos().x() <= (posX + width))
                        && (event->pos().y() >= posY) && (event->pos().y() <= (posY + height))) {
                        dragStartPosition = event->pos();
                        canDrag = 3;

                        QDrag *drag = new QDrag(this);
                        QMimeData *mimeData = new QMimeData;

                        drag->setMimeData(mimeData);
                        //drag->setPixmap(*(ui->slEdge->pixmap()));
                        Qt::DropAction dropAction = drag->exec();
                    } else {
                        height = ui->irEdge->height();
                        width = ui->irEdge->width();
                        posY = ui->irEdge->y();
                        posX = ui->irEdge->x();
                        if (event->button() == Qt::LeftButton && (event->pos().x() >= posX) && (event->pos().x() <= (posX + width))
                            && (event->pos().y() >= posY) && (event->pos().y() <= (posY + height))) {
                            dragStartPosition = event->pos();
                            canDrag = 4;

                            QDrag *drag = new QDrag(this);
                            QMimeData *mimeData = new QMimeData;

                            drag->setMimeData(mimeData);
                            //drag->setPixmap(*(ui->irEdge->pixmap()));
                            Qt::DropAction dropAction = drag->exec();
                        } else {
                            height = ui->ilEdge->height();
                            width = ui->ilEdge->width();
                            posY = ui->ilEdge->y();
                            posX = ui->ilEdge->x();
                            if (event->button() == Qt::LeftButton && (event->pos().x() >= posX) && (event->pos().x() <= (posX + width))
                                && (event->pos().y() >= posY) && (event->pos().y() <= (posY + height))) {
                                dragStartPosition = event->pos();
                                canDrag = 5;

                                QDrag *drag = new QDrag(this);
                                QMimeData *mimeData = new QMimeData;

                                drag->setMimeData(mimeData);
                                //drag->setPixmap(*(ui->ilEdge->pixmap()));
                                Qt::DropAction dropAction = drag->exec();
                            }
                        }
                    }
                }
            } else {
                canDrag = 0;
            }
        }
    } else {
        canDrag = 0;
    }
}


void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    QImage img, showed_img;
    int imgX, imgY, imgH, imgW, labelH, labelW;
    if(canDrag == 1){
        if (!(event->buttons() & Qt::LeftButton))
            return;
        if ((event->pos() - dragStartPosition).manhattanLength()
             < QApplication::startDragDistance())
            return;

        QDrag *drag = new QDrag(this);
        QMimeData *mimeData = new QMimeData;

        //mimeData->setData(mimeType, data);
        mimeData->setImageData((ui->foreground->pixmap())->toImage());
        drag->setMimeData(mimeData);

        Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction);
    } else {
        switch(canDrag){
        case 2:
            img = (ui->noScale->pixmap())->toImage();
            ui->foreground->resize((dragStartPosition.x() - event->pos().x()), (dragStartPosition.y() - event->pos().y()));
            qInfo("drag Start Position: %d %d | %d %d",dragStartPosition.x(), event->pos().x(), dragStartPosition.y(),  event->pos().y() );
            labelH = ui->foreground->height();
            labelW = ui->foreground->width();
            showed_img = img.scaled(labelW,labelH,Qt::KeepAspectRatio);
            ui->foreground->setPixmap(QPixmap::fromImage(showed_img));

            imgX = ui->foreground->x();
            imgY = ui->foreground->y();
            imgH = ui->foreground->pixmap()->height();
            imgW = ui->foreground->pixmap()->width();
            ui->srEdge->raise();
            ui->slEdge->raise();
            ui->irEdge->raise();
            ui->ilEdge->raise();
            ui->srEdge->setGeometry((imgX + imgW - 5), (imgY - 5), 15, 15);
            ui->slEdge->setGeometry((imgX - 5), (imgY - 5), 10, 10);
            ui->irEdge->setGeometry((imgX + imgW - 5), (imgY + imgH - 5), 10, 10);
            ui->ilEdge->setGeometry((imgX - 5), (imgY + imgH - 5), 10, 10);
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;

        }
    }
}

void MainWindow::dragMoveEvent(QDragMoveEvent *event)
{
    QImage img, showed_img;
    int imgX, imgY, imgH, imgW, labelH, labelW;
    qInfo("drag move");
    if (event->answerRect().intersects(ui->background->geometry()))

        switch(canDrag){
        case 2:
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
                ui->slEdge->raise();
                ui->irEdge->raise();
                ui->ilEdge->raise();
                ui->srEdge->setGeometry(((event->pos().x() - imgX) - 5), (imgY - 5), 15, 15);
                ui->slEdge->setGeometry((imgX - 5), (imgY - 5), 10, 10);
                ui->irEdge->setGeometry(((event->pos().x() - imgX) - 5), ((event->pos().y() - imgY) - 5), 10, 10);
                ui->ilEdge->setGeometry((imgX - 5), ((event->pos().y() - imgY) - 5), 10, 10);
            }
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        }

        event->acceptProposedAction();
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
        event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event)
{   int height, width, posX, posY;
    qInfo("drop");
   /* if (event->source() == this && event->possibleActions() & Qt::MoveAction)
        return;
    if (event->proposedAction() == Qt::MoveAction) {
        event->acceptProposedAction();
        qInfo("moveu");
    } else if (event->proposedAction() == Qt::CopyAction) {
        event->acceptProposedAction();
        // Process the data from the event.
        qInfo("copiou");
    } else {
        // Ignore the drop.
        qInfo("else -  ignorou");
        return;
    }*/
    posX = event->pos().x();
    posY = event->pos().y();
    qInfo("x: %d\n",posX);
    qInfo("y: %d\n",posY);
    qInfo("image x: %d\n",ui->foreground->x());
    qInfo("image y: %d\n",ui->foreground->y());
    height = ui->foreground->height();
    width = ui->foreground->width();
    //ui->foreground->setGeometry(width, height, posX, posY);
    ui->foreground->show();
    ui->foreground->setGeometry(posX, posY, width, height);
    ui->srEdge->show();
    ui->srEdge->setGeometry(((posX + width) - 5), (posY - 5), 15, 15);
    event->accept();
    //event->acceptProposedAction();
}
