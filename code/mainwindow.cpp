#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QPainter>
#include <QDebug>
#include <QTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MainwindowRunning();
}
// 绘制入口画面
void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap bg;
    bg.load(":/image/titlescreen.jpg");
    painter.drawPixmap(0,0,this->width(),this->height(),bg);
    bg.load(":/image/LoadBar_grass.png");
    painter.drawPixmap(250,500,bg);
    bg.load(":/image/LoadBar_dirt.png");
    painter.drawPixmap(250,520,bg);
    bg.load(":/image/pvz_logo.png");
    bg=bg.scaled(bg.width()*0.4,bg.height()*0.4);
    painter.drawPixmap(20,10,bg);

}
void MainWindow::MainwindowRunning()
{
    // 配置主窗口
    setFixedSize(800,594);
    setWindowIcon(QIcon(":/image/brain.png"));
    setWindowTitle("Plants vs Zombies test");
    ChooseScene=new choosescene;
    // 开始按钮
    startbtn = new QPushButton("CLICK IT TO START");
    startbtn->setParent(this);
    startbtn->adjustSize();
    startbtn->move(330,525);
    // 按钮样式表
    startbtn->setStyleSheet("QPushButton{background:transparent;color:rgb(240,230,140)}"
                            "QPushButton:hover{color: rgb(255,165,0)}"
                            "QPushButton:pressed{color: rgb(255,165,0)}"
                            );
    connect(startbtn,&QPushButton::clicked,[=](){
            ChooseScene->show();
            QTime dieTime = QTime::currentTime().addMSecs(400);
            while (QTime::currentTime() < dieTime)
                QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
            this->hide();
    });

    // 音乐播放
    player = new QMediaPlayer;
    player->setMedia(QUrl("qrc:/image/music/start_background.wav"));
    player->setVolume(40);
    player->play();

}
MainWindow::~MainWindow()
{
    delete ui;
}

