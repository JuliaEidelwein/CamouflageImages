#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QImage>
#include <QString>
#include <QDir>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->srEdge->setVisible(false);
    ui->slEdge->setVisible(false);
    ui->irEdge->setVisible(false);
    ui->ilEdge->setVisible(false);
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
    ui->noScale->setPixmap(QPixmap::fromImage(img));
    ui->noScale->setVisible(false);
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
        labelH = ui->background->height()/2;
        labelW = ui->background->width()/2;
        showed_img = img.scaled(labelW,labelH,Qt::KeepAspectRatio);
        ui->foreground->setGeometry(50,50,labelW,labelH);
        ui->foreground->setPixmap(QPixmap::fromImage(showed_img));
        ui->foreground->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
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
    int labelH, labelW, imgX, imgY, imgH, imgW;
    if(loaded2 > 0){
        imgX = ui->foreground->x();
        imgY = ui->foreground->y();
        //imgH = ui->foreground->height();
        imgH = ui->foreground->pixmap()->height();
        //imgW = ui->foreground->width();
        imgW = ui->foreground->pixmap()->width();
        ui->srEdge->setGeometry((imgX + imgW - 5), (imgY - 5), 10, 10);
        ui->slEdge->setGeometry((imgX - 5), (imgY - 5), 10, 10);
        ui->irEdge->setGeometry((imgX + imgW - 5), (imgY + imgH - 5), 10, 10);
        ui->ilEdge->setGeometry((imgX - 5), (imgY + imgH - 5), 10, 10);
        ui->srEdge->setVisible(true);
        ui->slEdge->setVisible(true);
        ui->irEdge->setVisible(true);
        ui->ilEdge->setVisible(true);
        //img = (ui->foreground->pixmap())->toImage();
        //ui->foreground->resize(100, 50);
        //labelH = ui->foreground->height();
        //labelW = ui->foreground->width();
        //showed_img = img.scaled(labelW,labelH,Qt::KeepAspectRatio);
        //ui->foreground->setPixmap(QPixmap::fromImage(showed_img));
    }
}
