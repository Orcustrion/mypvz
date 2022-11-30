#include "coloregg.h"
#include "ui_coloregg.h"

coloregg::coloregg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::coloregg)
{
    ui->setupUi(this);
    setFixedSize(800,594);
    setWindowIcon(QIcon(":/image/brain.png"));
    setWindowTitle("Plants vs Zombies test");

    // 开始按钮
    back_to_menu = new QPushButton;
    back_to_menu->setParent(this);
    back_to_menu->move(300,500);
    back_to_menu->setFixedSize(150,40);

    // 按钮样式表
    back_to_menu->setStyleSheet("QPushButton{border-image: url(:/image/SeedChooser_Button.png)}"
                            "QPushButton:hover{border-image: url(:/image/SeedChooser_Button2.png)}"
                            "QPushButton:pressed{border-image: url(:/image/SeedChooser_Button2.png)}"
                            );
    connect(back_to_menu,&QPushButton::clicked,[=](){
        emit this->signal_back();
    });

    back_to_menu_label= new QLabel("MAIN MENU",this);
    back_to_menu_label->move(340,510);
    back_to_menu->setFixedSize(150,40);
    back_to_menu_label->setStyleSheet("QLabel{background:transparent;color:rgb(165,42,42)}");
}

coloregg::~coloregg()
{
    delete ui;
}
