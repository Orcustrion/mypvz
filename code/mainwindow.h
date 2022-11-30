#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "choosescene.h"
#include <QMainWindow>
#include <QPushButton>
#include <QtMultimedia/QMediaPlayer>
#include<QtMultimedia/QMediaPlaylist>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void paintEvent(QPaintEvent *);
    choosescene * ChooseScene= nullptr;

    QPushButton * startbtn = nullptr;
    void MainwindowRunning();
    QMediaPlayer * player;

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
