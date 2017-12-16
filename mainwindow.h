#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    int loaded = 0;
    int loaded2 = 0;
    int canDrag = 0;
    int resizing = 0;
    QPoint dragStartPosition;
    QPoint dropPosition;
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_MainWindow_iconSizeChanged(const QSize &iconSize);

    void resizeEvent(QResizeEvent* ev);

    void mousePressEvent(QMouseEvent *event);

    void dropEvent(QDropEvent *event);

    void dragMoveEvent(QDragMoveEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void dragEnterEvent(QDragEnterEvent *event);

    void on_pushButton_6_clicked();

    void on_pushButton_6_clicked(bool checked);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
