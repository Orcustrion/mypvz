#include "choosescene.h"
#include "ui_choosescene.h"

#include <QPainter>
#include <QDebug>
#include <QIcon>
#include <QTime>

choosescene::choosescene(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::choosescene)
{
    ui->setupUi(this);
    ChooseSceneRunning();
}
void choosescene::ChooseSceneRunning()
{

    // 准备子界面
    Coloregg= new coloregg;

    // 配置主窗口
    setFixedSize(800,594);
    setWindowIcon(QIcon(":/image/brain.png"));
    setWindowTitle("Plants vs Zombies test");
    setMouseTracking(true);

    // 功能按钮
    flowerbutton_end = new QPushButton;
    flowerbutton_end->setParent(this);

    flowerbutton_end->move(722,508);
    flowerbutton_end->setStyleSheet("QPushButton{border-image: url(:/image/SelectorScreen_Quit1.png)}"
                            "QPushButton:hover{border-image: url(:/image/SelectorScreen_Quit2.png)}"
                            "QPushButton:pressed{border-image: url(:/image/SelectorScreen_Quit2.png)}"
                            );
    flowerbutton_end->setFixedSize(61,21);
    connect(flowerbutton_end,&QPushButton::clicked,[=](){
        this->close();
    });

    flowerbutton_help = new QPushButton;
    flowerbutton_help->setParent(this);

    flowerbutton_help->move(663,520);
    flowerbutton_help->setStyleSheet("QPushButton{border-image: url(:/image/SelectorScreen_Help1.png)}"
                            "QPushButton:hover{border-image: url(:/image/SelectorScreen_Help2.png)}"
                            "QPushButton:pressed{border-image: url(:/image/SelectorScreen_Help2.png)}"
                            );
    flowerbutton_help->setFixedSize(40,21);
    connect(flowerbutton_help,&QPushButton::clicked,[=](){
        Coloregg->show();
        this->hide();
    });
    connect(Coloregg,&coloregg::signal_back,this,[=](){
        this->show();
        Coloregg->hide();
    });

    flowerbutton_options = new QPushButton;
    flowerbutton_options->setParent(this);

    flowerbutton_options->move(583,497);
    flowerbutton_options->setStyleSheet("QPushButton{border-image: url(:/image/SelectorScreen_Options1.png)}"
                            "QPushButton:hover{border-image: url(:/image/SelectorScreen_Options2.png)}"
                            "QPushButton:pressed{border-image: url(:/image/SelectorScreen_Options2.png)}"
                            );
    flowerbutton_options->setFixedSize(75,21);
    connect(flowerbutton_options,&QPushButton::clicked,[=](){
        //this->close();
    });

    // 选关按钮
    Game_control=new game_control; // 创建gamecontrol对象

    gravebutton_1 = new QPushButton;
    gravebutton_1->setParent(this);

    gravebutton_1->move(440,80);
    gravebutton_1->setStyleSheet("QPushButton{border-image: url(:/image/SelectorScreen_StartAdventure_Button1.png);}"
                            "QPushButton:hover{border-image: url(:/image/SelectorScreen_StartAdventure_Highlight.png)}"
                            "QPushButton:pressed{border-image: url(:/image/SelectorScreen_StartAdventure_Highlight.png);}"
                            );
    gravebutton_1->setFixedSize(305,130);
    connect(gravebutton_1,&QPushButton::pressed,[=](){
        gravebutton_1->move(442,82);
    });
    connect(gravebutton_1,&QPushButton::released,[=](){
        gravebutton_1->move(440,80);
    });
    connect(gravebutton_1,&QPushButton::clicked,[=](){
        Game_control->initial_game_controler(1,1,1);
        Game_control->game_control_running();
        //Game_control->show();
        this->hide();
    });

    gravebutton_2 = new QPushButton;
    gravebutton_2->setParent(this);

    gravebutton_2->move(440,177);
    gravebutton_2->setStyleSheet("QPushButton{border-image: url(:/image/SelectorScreen_Survival_button.png);}"
                            "QPushButton:hover{border-image: url(:/image/SelectorScreen_Survival_highlight.png)}"
                            "QPushButton:pressed{border-image: url(:/image/SelectorScreen_Survival_highlight.png);}"
                            );
    gravebutton_2->setFixedSize(290,130);
    connect(gravebutton_2,&QPushButton::pressed,[=](){
        gravebutton_2->move(442,179);
    });
    connect(gravebutton_2,&QPushButton::released,[=](){
        gravebutton_2->move(440,177);
    });
    connect(gravebutton_2,&QPushButton::clicked,[=](){
        Game_control->show();
        this->hide();
    });

    gravebutton_3 = new QPushButton;
    gravebutton_3->setParent(this);

    gravebutton_3->move(445,260);
    gravebutton_3->setStyleSheet("QPushButton{border-image: url(:/image/SelectorScreen_Challenges_button.png);}"
                            "QPushButton:hover{border-image: url(:/image/SelectorScreen_Challenges_highlight.png)}"
                            "QPushButton:pressed{border-image: url(:/image/SelectorScreen_Challenges_highlight.png);}"
                            );
    gravebutton_3->setFixedSize(265,130);
    connect(gravebutton_3,&QPushButton::pressed,[=](){
        gravebutton_3->move(447,262);
    });
    connect(gravebutton_3,&QPushButton::released,[=](){
        gravebutton_3->move(445,260);
    });
    connect(gravebutton_3,&QPushButton::clicked,[=](){
        Game_control->show();
        this->hide();
    });

    gravebutton_4 = new QPushButton;
    gravebutton_4->setParent(this);

    gravebutton_4->move(445,335);
    gravebutton_4->setStyleSheet("QPushButton{border-image: url(:/image/SelectorScreen_Vasebreaker_button.png);}"
                            "QPushButton:hover{border-image: url(:/image/SelectorScreen_vasebreaker_highlight.png)}"
                            "QPushButton:pressed{border-image: url(:/image/SelectorScreen_vasebreaker_highlight.png);}"
                            );
    gravebutton_4->setFixedSize(250,120);
    connect(gravebutton_4,&QPushButton::pressed,[=](){
        gravebutton_4->move(447,337);
    });
    connect(gravebutton_4,&QPushButton::released,[=](){
        gravebutton_4->move(445,335);
    });
    connect(gravebutton_4,&QPushButton::clicked,[=](){
        Game_control->show();
        this->hide();
    });

    // 存档按钮
    record_button = new QPushButton;
    record_button->setParent(this);
    record_button->move(50,140);
    record_button->setStyleSheet("QPushButton{border-image: url(:/image/SelectorScreen_WoodSign2.png);}"
                            "QPushButton:hover{border-image: url(:/image/SelectorScreen_WoodSign2_press.png)}"
                            "QPushButton:pressed{border-image: url(:/image/SelectorScreen_WoodSign2_press.png);}"
                            );
    record_button->setFixedSize(260,80);
}
choosescene::~choosescene()
{
    delete ui;
}
