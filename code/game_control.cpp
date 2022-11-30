#include "game_control.h"
#include "ui_game_control.h"

#include <QDebug>
#include <QPainter>
#include <QString>

using namespace std;

game_control::game_control(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::game_control)
{
    ui->setupUi(this);
    window_setting();
}
void game_control::game_control_running()
{
    this->show();
    select_plants();
    start_anime();
}
void game_control::initial_game_controler(int gmode,int glevel,int gmapkind)
{
    // 初始化成员变量
    mode=gmode;
    level=glevel;
    mapkind=gmapkind;

    start_time=time(nullptr)*1000;
    sum_flame=0;

    inter=20;
    sun_inter=300;

    // 其他成员变量
    is_planting=false;
    is_planting_kind=0;
    is_shoveling=false;
    is_gloving=0;
    is_gloving_seq=0;
    is_running=1;

    wave=0;
    waving=new int[9]{0};

    sunshine = 500;
    intervel=0;

    shovel_change=0;
    glove_change=0;
    sunshine_change=0;

    if(gmapkind == 1)
    {
        // 前院白天 5行9列
        // 可读取文件  生成自定义墓碑位置
        rownum=5;
        plants_field = new int * [9];
        for(int i=0;i<9;i++)
            plants_field[i]=new int [5];
        for(int i=0;i<9;i++)
            for(int j=0;j<5;j++)
                plants_field[i][j]=0;

        read_field_info();


        // 初始化边界数组
        rects_rowfield=new int[6]{80,179,281,388,475,582};  // 纵坐标
        rects_rankfield=new int[10]{10,98,183,273,344,432,509,588,675,767};  // 横坐标

    }
    // 读取初始化信息
    read_plants_info();
    read_zombies_info();
}
void game_control::read_plants_info()
{
    // 目前完成2个植物
    // 格式：耐久 cd 阳光花费
    plant_kinds=8;
    plants_info = new int* [plant_kinds+1];
    for(int i=0;i<=plant_kinds;i++)
        plants_info[i]=new int[3];
    for(int i=0;i<=plant_kinds;i++)
        for(int j=0;j<3;j++)
            plants_info[i][j]=0;

    // 装载对应植物函数
    plants_functions[1]=&game_control::plant_1;
    plants_functions[2]=&game_control::plant_2;
    plants_functions[3]=&game_control::plant_3;
    plants_functions[4]=&game_control::plant_4;
    plants_functions[5]=&game_control::plant_5;
    plants_functions[6]=&game_control::plant_6;
    plants_functions[7]=&game_control::plant_7;
    plants_functions[8]=&game_control::plant_8;


    // 读取配置文件
    QFile file(":/plants_info.txt");
    QTextStream text(&file);
    int counter = 1;
    if(file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QString line = text.readLine();

        while(!text.atEnd())
        {
             QString line=text.readLine();
             QStringList strlist=line.split(" ");
             for(int i=0;i<3;i++)
             {
                 plants_info[counter][i]=strlist[i].toInt();
                 qDebug()<<strlist[i].toInt();
             }
             counter+=1;
        }
    }


}
void game_control::read_zombies_info()
{
    // 目前完成5个僵尸
    // 格式：生命值 攻击力 速度
    zombie_kinds=5;
    zombies_info = new int* [zombie_kinds];
    for(int i=0;i<zombie_kinds;i++)
        zombies_info[i]=new int[3];
    for(int i=0;i<zombie_kinds;i++)
        for(int j=0;j<3;j++)
            zombies_info[i][j]=0;

    // 读取配置文件
    QFile file(":/zombies_info.txt");
    QTextStream text(&file);
    int counter = 0;
    if(file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QString line = text.readLine();

        while(!text.atEnd())
        {
             QString line=text.readLine();
             QStringList strlist=line.split(" ");
             for(int i=0;i<3;i++)
             {
                 zombies_info[counter][i]=strlist[i].toInt();
                 qDebug()<<strlist[i].toInt();
             }
             counter+=1;
        }
    }
}
void game_control::read_field_info()
{
    QFile file(":/field_info.txt");
    QTextStream text(&file);
    if(file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QString line = text.readLine();
        while(!text.atEnd())
        {
             QString line=text.readLine();
             QStringList strlist=line.split(" ");

             if(strlist[0].toInt()==-1)  // 墓碑
             {
                 if(strlist[1].toInt()>=3 && strlist[1].toInt()<=8)
                 {
                     if(strlist[2].toInt()<=4)
                     {
                          qDebug()<<"!!"<<strlist;
                         plants_field[strlist[1].toInt()][strlist[2].toInt()]=-1;
                     }
                 }

             }
             else
             {

             }
        }
    }
}
void game_control::window_setting()
{
    // 配置窗口
    setFixedSize(800,594);
    setWindowIcon(QIcon(":/image/brain.png"));
    setWindowTitle("Plants vs Zombies test");

    // 其他设置
    setMouseTracking(true);

}
void game_control::is_planting_change(int i)
{

    if (is_planting == false)
    {
        int cost = plants_info[selection_kinds[i]][2];
        if (sunshine>=cost && planting_cd[i]==0)
            is_planting  =true;
        is_planting_kind=selection_kinds[i];
    }
    else if (is_planting == true)
        is_planting = false;

}
void game_control::select_plants()
{
    // 选择植物流程

    // 测试简易实现  test
    selection_number=8;
    selection_kinds=new int[selection_number];
    planting_cd=new int[selection_number]{0};
    planting_b=new QLabel *[selection_number];

    // 暂定
    for(int i=0;i<selection_number;i++)
    {
        planting_b[i]=NULL;
        selection_kinds[i]=i+1;
    }

    // 按钮相关设定
    seed_buttons = new QPushButton* [selection_number];
    for(int i=0;i<selection_number;i++)
    {
        // 按钮样式设定
        QPushButton * ss = new  QPushButton;
        ss->move(90+51*i,5);
        ss->setFixedSize(51,68);
        QString stylestr= "QPushButton{border-image: url(:/image/image_1/seed_bank_";
        stylestr.append(QString::number(selection_kinds[i]));
        stylestr.append(".png)}");
        ss->setStyleSheet(stylestr);

        seed_buttons[i]=ss;
        seed_buttons[i]->setParent(this);
        seed_buttons[i]->show();

        // 设置连接函数
        connect(seed_buttons[i],&QPushButton::clicked,[=](){
            this->is_planting_change(i);
        });

    }

    // 配置阳光栏
    sunshine_view = new QLabel;
    sunshine_view->move(45,60);
    sunshine_view->setFixedSize(30,10);
    sunshine_view->setText(QString::number(sunshine));
    sunshine_view->setStyleSheet("QLabel{font:bold 14px;}");
    sunshine_view->setParent(this);
    sunshine_view->show();

    // 配置铲子
    Shovel = new shovel();
    Shovel->shovel_button->setParent(this);
    Shovel->shovel_button->show();
    connect(Shovel->shovel_button,&QPushButton::clicked,[=](){
        is_shoveling=true;
        Shovel->shovel_button->hide();
    });

    // 配置手套
    Glove = new glove();
    Glove->glove_button->setParent(this);
    Glove->glove_button->show();
    connect(Glove->glove_button,&QPushButton::clicked,[=](){
        is_gloving=1;
        Glove->glove_button->hide();
    });

    // 配置波数栏
    wave_view = new QLabel;
    wave_view->move(680,575);
    wave_view->setFixedSize(120,20);
    QString t="当前波数："+QString::number(wave)+"/15";
    wave_view->setText(t);
    wave_view->setStyleSheet("QLabel{font:bold 14px;}");
    wave_view->setParent(this);
    wave_view->show();

    // 配置暂停键
    pause = new QPushButton();
    pause->move(750,0);
    pause->setFixedSize(50,50);

    pause->setStyleSheet("QPushButton{border-image: url(:/image/pause.png)}");
    pause->setParent(this);
    pause->show();
    connect(pause,&QPushButton::clicked,[=](){
       if(is_running==1)
       {
           game_timer->stop();
           pause->setStyleSheet("QPushButton{border-image: url(:/image/start.png)}");
           is_running=0;
       }
       else if(is_running==0)
       {
           game_timer->start();
           pause->setStyleSheet("QPushButton{border-image: url(:/image/pause.png)}");
           is_running=1;
       }
    });

    // 配置buff按钮
    buff_buttons = new QPushButton* [7];
    for(int i=0;i<7;i++)
    {
        QPushButton * bb = new QPushButton();
        bb->setFixedSize(150,40);
        bb->setStyleSheet("QPushButton{border-image: url(:/image/SeedChooser_Button.png)}"
                          "QPushButton:hover{border-image: url(:/image/SeedChooser_Button2.png)}"
                          "QPushButton:pressed{border-image: url(:/image/SeedChooser_Button2.png)}"
                          );
        buff_buttons[i]=bb;
        buff_buttons[i]->setParent(this);


    }
    // 设置连接函数
    buff_buttons[0]->setText("无");
    connect(buff_buttons[0],&QPushButton::clicked,[=](){
        game_timer->start();
        for(int i=0;i<7;i++)
            buff_buttons[i]->hide();
    });
    buff_buttons[1]->setText("自然产出阳光速度加快");
    connect(buff_buttons[1],&QPushButton::clicked,[=](){
        game_timer->start();
        for(int i=0;i<7;i++)
            buff_buttons[i]->hide();
        sun_inter=200;
    });
    buff_buttons[2]->setText("自然产出阳光数值增加");
    connect(buff_buttons[2],&QPushButton::clicked,[=](){
        game_timer->start();
        for(int i=0;i<7;i++)
            buff_buttons[i]->hide();
        sunshine_change=1;
    });
    buff_buttons[3]->setText("使用铲子返还50阳光");
    connect(buff_buttons[3],&QPushButton::clicked,[=](){
        game_timer->start();
        for(int i=0;i<7;i++)
            buff_buttons[i]->hide();
        shovel_change=1;
    });
    buff_buttons[4]->setText("使用手套不消耗阳光");
    connect(buff_buttons[4],&QPushButton::clicked,[=](){
        game_timer->start();
        for(int i=0;i<7;i++)
            buff_buttons[i]->hide();
        glove_change=1;
    });
    buff_buttons[5]->setText("《僵尸快跑》");
    connect(buff_buttons[5],&QPushButton::clicked,[=](){
        game_timer->start();
        for(int i=0;i<7;i++)
            buff_buttons[i]->hide();
        inter=15;
    });
    buff_buttons[6]->setText("《僵尸慢跑》");
    connect(buff_buttons[6],&QPushButton::clicked,[=](){
        game_timer->start();
        for(int i=0;i<7;i++)
            buff_buttons[i]->hide();
        inter=25;
    });



}

void game_control::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    // 场景绘画

    if (mapkind == 1) //前院白天
    {
        QPixmap bg;
        bg.load(":/image/image_1/background1.png");
        painter.drawPixmap(0,10,this->width(),this->height(),bg);
        QPixmap sb;
        sb.load(":/image/image_1/SeedBank.png");
        sb=sb.scaled(sb.width(),sb.height()*0.9);
        painter.drawPixmap(15,0,sb);

        // 画墓碑
        for(int i=0;i<9;i++)
            for(int j=0;j<5;j++)
                if(plants_field[i][j]==-1)
                {
                    QPixmap tb;
                    tb.load(":/image/image_3/tomb.png");
                    tb=tb.scaled(tb.width()*0.5,tb.height()*0.5);
                    painter.drawPixmap(rects_rankfield[i],rects_rowfield[j],tb);
                }
    }
    else
    {

    }

    // 铲子框
    QPixmap sh;
    sh.load(":/image/ShovelBank.png");
    painter.drawPixmap(500,0,sh);
    // 手套框
    QPixmap sh1;
    sh1.load(":/image/ShovelBank.png");
    painter.drawPixmap(570,0,sh1);


    // 种植时特效
    if (is_planting == true)
    {
        QPixmap pl;
        QString plstr=":/image/image_1/planting_";
        plstr.append(QString::number(is_planting_kind));
        plstr.append(".png");
        pl.load(plstr);
        painter.drawPixmap(temp_x-30,temp_y-60,pl);
    }

    // 铲子特效
    if (is_shoveling == true)
    {
        QPixmap shing;
        shing.load(":/image/Shovel_hi_res.png");
        shing=shing.scaled(shing.width()*0.6,shing.height()*0.6);
        painter.drawPixmap(temp_x-30,temp_y-60,shing);
    }

    // 手套特效
    if(is_gloving!=0)
    {
        QPixmap gling;
        gling.load(":/image/glove.png");
        gling=gling.scaled(gling.width()*0.25,gling.height()*0.25);
        painter.drawPixmap(temp_x-30,temp_y-30,gling);
    }

    // 受击特效
    for(int i=0;i<conflicts.size();i++)
    {
        if(conflicts[i].kind==1 && conflicts[i].life>0)
        {
            QPixmap s1;
            s1.load(":/image/blooddrop.png");
            s1=s1.scaled(s1.width()*0.25,s1.height()*0.25);
            painter.drawPixmap(conflicts[i].pos_x,conflicts[i].pos_y,s1);
        }
        else if(conflicts[i].kind==2 && conflicts[i].life>0)
        {
            QPixmap s2;
            s2.load(":/image/waterdrop.png");
            painter.drawPixmap(conflicts[i].pos_x,conflicts[i].pos_y,s2);
        }
    }
}

// 鼠标事件函数
void game_control::mousePressEvent(QMouseEvent * event)
{
    temp_x=event->pos().x();
    temp_y=event->pos().y();

    // 鼠标点击：种植
    if (is_planting == true)
    {
        int* ans=where_in_feild(temp_x,temp_y);
        qDebug()<<ans[0]<<" "<<ans[1];
        if (ans[0]!=-1) // 合法位置
        {
            if(plants_field[ans[0]][ans[1]]!=-1)
            {
                plants_field[ans[0]][ans[1]]=is_planting_kind;
                planting(is_planting_kind,ans[0],ans[1]); // 种类 位置
                is_planting_kind=0;
            }
        }
        is_planting=false; // 种植行为结束
        update();
        temp_x=0;
        temp_y=0;
    }

    // 鼠标点击：铲
    if(is_shoveling == true)
    {
        int* ans=where_in_feild(temp_x,temp_y);
        qDebug()<<ans[0]<<" "<<ans[1];
        if (ans[0]!=-1) // 合法位置
            if(plants_field[ans[0]][ans[1]]!=-1)
            {
                del_plants(ans[0],ans[1]);
                if(shovel_change==1)
                {
                    sunshine+=50;
                    sunshine_view->setText(QString::number(sunshine));
                }
            }
        is_shoveling=false; // 铲行为结束
        Shovel->shovel_button->show();
        update();
        temp_x=0;
        temp_y=0;

    }

    // 鼠标点击：手套
    if(is_gloving==1)
    {
        int* ans=where_in_feild(temp_x,temp_y);
        qDebug()<<ans[0]<<" "<<ans[1];
        if (ans[0]!=-1) // 合法位置
        {
            if(plants_field[ans[0]][ans[1]]>0)
            {
                for(int i=0;i<plants_vector.size();i++)
                    if(plants_vector[i]->px==ans[0] && plants_vector[i]->py==ans[1])
                        is_gloving_seq=i;

                is_gloving=2;
            }
            else
                is_gloving=0;
        }
        else
            is_gloving=0;

        if(is_gloving==0)
        {
            temp_x=0;
            temp_y=0;
            Glove->glove_button->show();
        }
        update();
    }

    else if(is_gloving==2)
    {
        int* ans=where_in_feild(temp_x,temp_y);
        qDebug()<<ans[0]<<" "<<ans[1];
        if (ans[0]!=-1 && sunshine>=50) // 合法位置
            if(plants_field[ans[0]][ans[1]]!=-1 && is_gloving_seq<plants_vector.size())
            {
                planting(plants_vector[is_gloving_seq]->kind,ans[0],ans[1]);
                sunshine+=plants_vector[is_gloving_seq]->cost;
                if(glove_change==0)
                    sunshine-=50;
                sunshine_view->setText(QString::number(sunshine));

                plants_field[plants_vector[is_gloving_seq]->px][plants_vector[is_gloving_seq]->py]=0;
                plants_vector[is_gloving_seq]->plant_gif->deleteLater();
                plants_vector.erase(plants_vector.begin()+is_gloving_seq);


            }
        is_gloving=0;
        Glove->glove_button->show();
        update();
        temp_x=0;
        temp_y=0;
    }

}
void game_control::mouseMoveEvent(QMouseEvent *event)
{
    // 追踪鼠标移动轨迹
    if (is_planting == true)
    {
        temp_x=event->pos().x();
        temp_y=event->pos().y();
        update();
    }

    if (is_shoveling == true)
    {
        temp_x=event->pos().x();
        temp_y=event->pos().y();
        update();
    }

    if (is_gloving != 0)
    {
        temp_x=event->pos().x();
        temp_y=event->pos().y();
        update();
    }

}
void game_control::mouseReleaseEvent(QMouseEvent * event)
{


}


void game_control::planting(int kind,int gx,int gy)
{
    // 根据种类创建对象
    int offset_x=0,offset_y=0;
    switch (kind) {
    case 1: // sunflower
    {
        sunflower * SunFlower = new sunflower();
        offset_x=5;
        offset_y=5;
        plants_vector.push_back(SunFlower);
        break;
    }
    case 2: // pea shooter
    {
        peashooter * PeaShooter = new peashooter();
        offset_x=10;
        offset_y=5;
        plants_vector.push_back(PeaShooter);
        break;
    }
    case 3: // wallnut
    {
        wallnut * WallNut = new wallnut();
        offset_x=5;
        offset_y=10;
        plants_vector.push_back(WallNut);
        break;
    }
    case 4: // chomper
    {
        chomper * Chomper =new chomper();
        offset_x=5;
        offset_y=-10;
        plants_vector.push_back(Chomper);
        break;
    }
    case 5: // spikeweed
    {
        spikeweed * SpikeWeed =new spikeweed();
        offset_x=0;
        offset_y=50;
        plants_vector.push_back(SpikeWeed);
        break;
    }
    case 6: // cherrybomb
    {
        cherrybomb * CherryBomb=new cherrybomb();
        plants_vector.push_back(CherryBomb);
        break;
    }
    case 7: // snowpea
    {
        snowpea * SnowPea=new snowpea();
        plants_vector.push_back(SnowPea);
        break;
    }
    case 8: // cactus
    {
        cactus * Cactus=new cactus();
        offset_x=10;
        offset_y=5;
        plants_vector.push_back(Cactus);
        break;
    }
    default:
    {
        break;
    }
    }


    plants * new_plant=plants_vector[plants_vector.size()-1];

    // 设定相关数值
    new_plant->plant_init(plants_info[kind][0],plants_info[kind][1],plants_info[kind][2]);
    new_plant->set_plant_info(gx,gy,sum_flame);
    new_plant->plant_gif->setParent(this);
    // 设定植物待机gif
    new_plant->plant_gif->move(rects_rankfield[gx]+offset_x,rects_rowfield[gy]+offset_y);
    new_plant->display();

    // 设置阳光 cd
    sunshine-=new_plant->cost;
    sunshine_view->setText(QString::number(sunshine));
    planting_cd[kind-1]=plants_info[kind][1];
    plants_field[gx][gy]=kind;

}
void game_control::del_plants(int x, int y)
{
    int i=0;
    for(;i<plants_vector.size();i++)
    {
        if(plants_vector[i]->px==x && plants_vector[i]->py ==y)
            break;
    }
    if(i<plants_vector.size())
    {
        plants_vector[i]->plant_gif->deleteLater();
        plants_vector.erase(plants_vector.begin()+i);
        plants_field[x][y]=0;
    }
}
void game_control::plant_1(int state, int seq)
{
    if(state==1)
    {
        produce_SunShine(2,seq);
    }
    else if(state==-1)
    {
        del_plants(plants_vector[seq]->px,plants_vector[seq]->py);
    }

}
void game_control::plant_2(int state, int seq)
{
    if(state==-1)
    {
        del_plants(plants_vector[seq]->px,plants_vector[seq]->py);
    }
    else if(state==1)
    {
        add_newbullet(1,plants_vector[seq]->px,plants_vector[seq]->py);
    }
}
void game_control::plant_3(int state, int seq)
{
    if(state==-1)
    {
       del_plants(plants_vector[seq]->px,plants_vector[seq]->py);
    }
}
void game_control::plant_4(int state, int seq)
{
    if(state==-1)
    {
       del_plants(plants_vector[seq]->px,plants_vector[seq]->py);
    }
    else if(state==2)
    {
        int i=0;
        for(;i<zombies_vector.size();i++)
        {
            if(plants_vector[seq]->px==zombies_vector[i]->pos &&
                 plants_vector[seq]->py==zombies_vector[i]->row   )
                break;
        }
        if(i!=zombies_vector.size())
            del_zombie(i);
        else
        {
            i=0;
            for(;i<zombies_vector.size();i++)
            {
                if(plants_vector[seq]->px+1==zombies_vector[i]->pos &&
                     plants_vector[seq]->py==zombies_vector[i]->row   )
                    break;
            }
            del_zombie(i);
        }
    }

}
void game_control::plant_5(int state, int seq)
{
    spikeweed * tmp=(spikeweed *)plants_vector[seq];
    if(state==-1)
    {
       del_plants(tmp->px,tmp->py);
    }
    else if(state==1)
    {

        int rec_left=rects_rankfield[tmp->px]-20,rec_right=rects_rankfield[tmp->px+1]+20;
        for(int i=0;i<zombies_vector.size();i++)
        {
            if(zombies_vector[i]->row==tmp->py &&
                    (zombies_vector[i]->rect<=rec_right && zombies_vector[i]->rect>=rec_left))
            {
                zombies_vector[i]->lifepoint-=tmp->damage;
            }
        }
    }


}
void game_control::plant_6(int state, int seq)
{
    cherrybomb * tmp=(cherrybomb*)plants_vector[seq];
    if(state==-1)
    {
       del_plants(plants_vector[seq]->px,plants_vector[seq]->py);
    }
    else if(state==1)
    {
       int x=tmp->px;
       int y=tmp->py;
       for(int i=0;i<zombies_vector.size();i++)
       {
           if(abs(zombies_vector[i]->row-y)<=1 && abs(zombies_vector[i]->pos-x)<=1)
           {
               zombies_vector[i]->lifepoint-=tmp->damage;
           }
       }
    }
}
void game_control::plant_7(int state, int seq)
{

}
void game_control::plant_8(int state, int seq)
{
    if(state==-1)
    {
        del_plants(plants_vector[seq]->px,plants_vector[seq]->py);
    }
    else if(state==1)
    {
        add_newbullet(2,plants_vector[seq]->px,plants_vector[seq]->py);
    }
}

void game_control::produce_SunShine(int kind,int seq)
{
    SunShine * newsun;
    if(kind == 1)  // 自然生产阳光
    {
        int pos=(rand() % (700-50))+ 50;  // 随机数
        newsun=new SunShine(pos,0,kind,sum_flame);
        if(sunshine_change==1)
            newsun->sun_number+=25;
    }
    else if(kind == 2) // 植物产生阳光
    {
        int px=rects_rankfield[(plants_vector[seq]->px)]+20;
        int py=rects_rowfield[(plants_vector[seq]->py)]+20;
        newsun=new SunShine(px,py,kind,sum_flame);

    }
    sunshine_vector.push_back(newsun);

    newsun->instructor->setParent(this);
    newsun->sun_gif->setParent(this);
    connect(newsun->instructor,&QPushButton::clicked,[=](){
        sunshine+=newsun->get_number();
        sunshine_view->setText(QString::number(sunshine));
        for(int i=0;i<sunshine_vector.size();i++)
            if(sunshine_vector[i]==newsun)
            {
                sunshine_vector.erase(sunshine_vector.begin()+i);
                break;
            }
        newsun->instructor->deleteLater();
        delete newsun;
    });
    newsun->display();
}



void game_control::control_newzombie()
{
    /* 僵尸
     * 普通：0
     * 路障：1
     * 铁桶：2
     * 气球：3
     * 投石车：4
     */
    int change=0;

    if(waving[8]>0)
        waving[8]-=1;
    else
        change=1;
    if(zombies_vector.size()==0 && waving[6]==0)
    {
        change=1;   
    }
    if(change==1)   // update waving
    {
        // 0~4：僵尸概率比重 5：是否在刷僵尸 6：数量 7：出现间隔  8:波间隔

        wave+=1;
        get_buff(wave);

        waving[0]=60-wave;
        waving[1]=80-wave;
        waving[2]=90-wave;
        waving[3]=95-wave*0.5;
        waving[4]=100;
        waving[5]=1;
        if(wave%10==0)
        {
            waving[6]=20;
            waving[7]=5;

        }
        else
        {
            if(wave<=5)
            {
                waving[6]=4;
                waving[7]=100;
            }
            if(wave<=10 && wave>5)
            {
                waving[6]=7;
                waving[7]=80;
            }
            if(wave>10)
            {
                waving[6]=10;
                waving[7]=60;
            }
        }
        waving[8]=1000;
    }

    if(waving[5]==1 && waving[6]>0)  // 僵尸出现
    {
        intervel+=1;
        if(intervel==waving[7])
        {
            intervel=0;
            int new_z=rand()%100;
            int i=0;
            for(;i<5;i++)
                if(new_z<waving[i])
                {
                    add_newzombie(new_z%5,i);
                    break;
                }
            waving[6]-=1;
        }
    }

}
void game_control::add_newzombie(int row,int zkind)
{
   switch (zkind)
   {
   case 0:
   {
       normal_zombie * new_zombie=new normal_zombie();
       zombies_vector.push_back(new_zombie);
       break;
   }

   case 1:
   {
       conehead_zombie * new_zombie=new conehead_zombie();
       zombies_vector.push_back(new_zombie);
       break;
   }
   case 2:
   {
       buckethead_zombie * new_zombie=new buckethead_zombie();
       zombies_vector.push_back(new_zombie);
       break;
   }
   case 3:
   {
       balloon_zombie * new_zombie=new balloon_zombie();
       zombies_vector.push_back(new_zombie);
       break;
   }
   case 4:
   {
       catapult_zombie * new_zombie=new catapult_zombie();
       zombies_vector.push_back(new_zombie);
       break;
   }
   default:
       break;
   }

   // 设定相关数值
   zombies * temp_zombie=zombies_vector[zombies_vector.size()-1];
   temp_zombie->zombie_init(zombies_info[zkind][0],zombies_info[zkind][1],zombies_info[zkind][2]);
   temp_zombie->set_zombie_info(row,sum_flame);
   temp_zombie->px=temp_zombie->rect-60;
   temp_zombie->py=rects_rowfield[row]-50;
   temp_zombie->zombie_gif->setParent(this);


   // 设定gif
   temp_zombie->display();
}
void game_control::del_zombie(int seq)
{
     qDebug()<<"del";
     zombies_vector[seq]->zombie_gif->deleteLater();
     zombies_vector.erase(zombies_vector.begin()+seq);

}
void game_control::handle_catapult(int seq,int res)
{
    catapult_zombie * tmp=(catapult_zombie*)zombies_vector[seq];
    if(res>=0 && res<=9)
    {
        // 投石车发射石头
        catapultbullet * Catapult=new catapultbullet();
        bullets_vector.push_back(Catapult);
        Catapult->set_pos(tmp->px+100,tmp->py+20,tmp->rect+60,tmp->row);
        Catapult->bullet_pic->setParent(this);
        Catapult->bullet_pic->show();
        Catapult->pos=res;
        Catapult->calculate(rects_rankfield[res]+45);

    }
    else if(res==10)
    {
        // 投石车碾压植物
        for(int i=0;i<plants_vector.size();i++)
        {
            if(plants_vector[i]->px==tmp->pos && plants_vector[i]->py==tmp->row)
            {
                plants_vector[i]->hitpoint=0;
                if(plants_vector[i]->kind==5)
                 {
                    zombies_vector[seq]->lifepoint=0;
                }
            }
        }
    }
}


void game_control::add_newbullet(int kind, int px, int py)
{
    // 子弹种类 源坐标
    int offset_x,offset_y;
    switch (kind)
    {
    case 1: // 普通豌豆
    {
        offset_x=50;
        offset_y=10;
        peabullet * PeaBullet=new peabullet();
        bullets_vector.push_back(PeaBullet);
        break;
    }
    case 2: // 仙人掌刺
    {
        offset_x=50;
        offset_y=20;
        cactusbullet * CactusBullet=new cactusbullet();
        bullets_vector.push_back(CactusBullet);
        break;
    }
    case 3: // 投石车僵尸
    {

    }
    default:
        break;
    }
    bullet * temp=bullets_vector[bullets_vector.size()-1];


    temp->set_pos(rects_rankfield[px]+offset_x,rects_rowfield[py]+offset_y,rects_rankfield[px]+100,py);
    temp->bullet_pic->setParent(this);
    temp->bullet_pic->show();

}
void game_control::del_bullet(int seq)
{
    bullets_vector[seq]->bullet_pic->clear();
    bullets_vector.erase(bullets_vector.begin()+seq);
}


int * game_control::where_in_feild(int x, int y)
{
    int * answer = new int [2]{-1};
    for(int i=0;i<6;i++)
        if(y<=rects_rowfield[i])
        {
            answer[1]=i-1;
            break;
        }
    for(int i=0;i<10;i++)
        if(x<=rects_rankfield[i])
        {
            answer[0]=i-1;
            break;
        }
    if(answer[0]<0 || answer[1]<0)  // 非法位置
      {
        answer[0]=-1;
        answer[1]=-1;
      }
    return answer;
}


void game_control::start_anime()
{
    QPixmap pix;
    QElapsedTimer t;
    // 三个图片放入 ...
    QLabel* pic1=new QLabel();
    pix.load(":/image/image_1/startgame.png");
    pix=pix.scaled(pix.width()*0.3,pix.height()*0.3);
    pic1->setPixmap(pix);
    pic1->setParent(this);
    pic1->move(250,250);
    pic1->show();
    t.start();
    while(t.elapsed()<1000)
    QCoreApplication::processEvents();
    pic1->hide();

    QLabel* pic2=new QLabel();
    pix.load(":/image/image_1/startgame1.png");
    pix=pix.scaled(pix.width()*0.3,pix.height()*0.3);
    pic2->setPixmap(pix);
    pic2->setParent(this);
    pic2->move(250,250);
    pic2->show();
    t.start();
    while(t.elapsed()<1000)
    QCoreApplication::processEvents();
    pic2->hide();

    QLabel* pic3=new QLabel();
    pix.load(":/image/image_1/startgame2.png");
    pix=pix.scaled(pix.width()*0.3,pix.height()*0.3);
    pic3->setPixmap(pix);
    pic3->setParent(this);
    pic3->move(250,250);
    pic3->show();
    t.start();
    while(t.elapsed()<1000)
    QCoreApplication::processEvents();
    pic3->hide();


    // 游戏开始

    // 定时器
    qDebug()<<"begin";
    game_timer=new QTimer(this);
    game_timer->setInterval(inter);  // 20ms 刷新
    game_timer->start();
    connect(game_timer,&QTimer::timeout,this,[=](){
        main_circle();
    });

}
void game_control::main_circle()
{
    sum_flame+=1;
    // cd
    for(int i=0;i<selection_number;i++)
    {

        if(planting_cd[i]>0)
            planting_cd[i]-=1;
       if(sum_flame %5==0)
       {
            if(planting_b[i]!=NULL )
           {
                planting_b[i]->deleteLater();
                delete planting_b[i];
           }
           planting_b[i]=new QLabel();
           planting_b[i]->setPixmap(QPixmap(":/image/image_1/black.png"));
           planting_b[i]->setParent(this);
           planting_b[i]->move(90+53*i,5);
           planting_b[i]->resize(53,68*planting_cd[i]/plants_info[i+1][1]);
           planting_b[i]->show();
       }
    }


    // 阳光
    if (sum_flame % sun_inter ==0)  // 自然掉落阳光 6s一次
    {
        produce_SunShine(1,0);
    }
    for(int i=0;i<sunshine_vector.size();i++)
    {
        int sunshine_re=sunshine_vector[i]->update_state(sum_flame);
        if(sunshine_re==1)
        {
            sunshine_vector[i]->instructor->deleteLater();
            delete sunshine_vector[i];
            sunshine_vector.erase(sunshine_vector.begin()+i);
        }
    }

    // 植物
    for(int i=0;i<plants_vector.size();i++)
    {
        int tkind=plants_vector[i]->kind;
        int tstate=plants_vector[i]->update_state(zombies_vector);
        (this->*plants_functions[tkind])(tstate,i);
    }

    // 僵尸
    control_newzombie();
    for(int i=0;i<zombies_vector.size();i++)
    {
        zombies * tmp=zombies_vector[i];
        int res=tmp->update_state(plants_field);
        if(res==-1) // 死亡结束
        {
            del_zombie(i);
        }
        else if(res==1 && tmp->kind!=4) // 进攻
        {
            handle_attack(i);
        }
        else if(res==999) // 进入房子 游戏结束
        {
            game_end(0);    // 0 僵尸胜利
        }
        // 特殊：投石车僵尸 其他情况
        if(tmp->kind==4)
        {
            handle_catapult(i,res);
        }
    }

    // 子弹
    for(int i=0;i<bullets_vector.size();i++)
    {

        int res=bullets_vector[i]->update_state(zombies_vector);
        if(res==-1)  // 单独处理投石车僵尸
        {
            if(bullets_vector[i]->kind==3)
            {
              catapultbullet * tmp=(catapultbullet *)bullets_vector[i];
              for(int j=0;j<plants_vector.size();j++)
              {
                  if(tmp->row==plants_vector[j]->py && tmp->pos==plants_vector[j]->px)
                  {
                      plants_vector[j]->hitpoint-=tmp->damage;
                      conflict con(rects_rankfield[plants_vector[j]->px]+60,
                              rects_rowfield[plants_vector[j]->py]+40,2);
                      conflicts.push_back(con);
                  }
              }
            }
            del_bullet(i);

        }

        else if(res>0)
        {
            handle_bullet(res-1,i);
        }

    }

    // 受击特效
    for(int i=0;i<conflicts.size();i++)
    {
       conflicts[i].life-=1;
       if(conflicts[i].life<=0)
       {
           conflicts.erase(conflicts.begin()+i);
       }
    }

}
void game_control::game_end(int sign)
{
    game_timer->stop();
    qDebug()<<"end";
    if(sign==1)       // 植物胜利
    {
        QPushButton * end =new QPushButton();
        end->setStyleSheet("QPushButton{border-image: url(:/image/image_1/trophy.png)}");
        end->setParent(this);
        end->setFixedSize(170,150);
        end->move(200,200);
        end->show();
        connect(end,&QPushButton::clicked,[=](){
            this->hide();
        });
    }
    else if(sign==0)  // 僵尸胜利
    {
        QLabel * end = new QLabel();
        end->setPixmap(QPixmap(":/image/image_1/ZombiesWon.png"));
        end->setParent(this);
        end->move(150,100);
        end->show();
    }
}
void game_control::handle_attack(int seq)
{
    zombies * tmp=zombies_vector[seq];
    int damage=tmp->atk;
    for(int i=0;i<plants_vector.size();i++)
    {
        if(plants_vector[i]->px==tmp->pos && plants_vector[i]->py == tmp->row)
        {
            plants_vector[i]->hitpoint-=damage;
            conflict con(rects_rankfield[plants_vector[i]->px]+60,
                    rects_rowfield[plants_vector[i]->py]+40,2);
            conflicts.push_back(con);
        }
    }
}
void game_control::handle_bullet(int zseq,int bseq)
{
    zombies * tmp=zombies_vector[zseq];
    tmp->lifepoint-=bullets_vector[bseq]->damage;
    conflict con(tmp->px+60,tmp->py+40,1);
    conflicts.push_back(con);
}
void game_control::get_buff(int wave)
{
    QString t="当前波数："+QString::number(wave)+"/15";
    wave_view->setText(t);
    if(wave>=15)
        game_end(1);
    else if(wave==1)
    {
        game_timer->stop();
        buff_buttons[1]->move(50,10);
        buff_buttons[1]->show();
        buff_buttons[2]->move(260,10);
        buff_buttons[2]->show();
        buff_buttons[0]->move(470,10);
        buff_buttons[0]->show();
    }
    else if(wave==5)
    {
        game_timer->stop();
        buff_buttons[3]->move(50,10);
        buff_buttons[3]->show();
        buff_buttons[4]->move(260,10);
        buff_buttons[4]->show();
        buff_buttons[0]->move(470,10);
        buff_buttons[0]->show();
    }
    else if(wave==10)
    {
        game_timer->stop();
        buff_buttons[5]->move(50,10);
        buff_buttons[5]->show();
        buff_buttons[6]->move(260,10);
        buff_buttons[6]->show();
        buff_buttons[0]->move(470,10);
        buff_buttons[0]->show();
    }
}
game_control::~game_control()
{
    delete ui;
}
