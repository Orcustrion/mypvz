#include "assist.h"
#include <QDebug>
SunShine::SunShine()
{
    sunshine_kind=1;
    sun_number=50;
    exist_time=500;
    px=50;
    py=20;
}
SunShine::SunShine(int px,int py,int kind,int birth)
{
    sunshine_kind=kind;
    if(sunshine_kind == 1)
    {
        this->py=60;
        sun_number=25;
    }
    else
    {
        this->py=py;
        sun_number=25;
    }
    this->birth=birth;
    exist_time=500;
    this->px=px;

    instructor = new QPushButton();
    layout = new QVBoxLayout(instructor);
    sun_gif=new QLabel();


}
void SunShine::display()
{
    instructor->setStyleSheet("QPushButton{background:transparent}");
    sun_gif->setAlignment(Qt::AlignHCenter);
    sun_movie = new QMovie(":/image/image_1/sunshine.gif");

    layout->addWidget(sun_gif);
    sun_gif -> setMovie(sun_movie);

    instructor->move(px,py);
    sun_movie->start();
    sun_gif->show();
    instructor->setFixedSize(80,90);
    instructor->show();

}
int SunShine::update_state(int flame)
{
    if(flame-birth>=exist_time)
        return 1;
    else
    {
        if(py<520 && sunshine_kind==1)
        {
            py+=2;
            instructor->move(px,py);
        }
        return 2;
    }
}

int SunShine::get_number()
{
    return sun_number;
}




shovel::shovel()
{
    px=490;
    py=0;
    shovel_button=new QPushButton();
    shovel_button->setStyleSheet("QPushButton{border-image: url(:/image/Shovel.png)}");
    shovel_button->setFixedSize(80,80);
    shovel_button->move(px,py);
}

glove::glove()
{
    px=570;
    py=10;
    glove_button=new QPushButton();
    glove_button->setStyleSheet("QPushButton{border-image: url(:/image/glove.png)}");
    glove_button->setFixedSize(70,70);
    glove_button->move(px,py);
}

bullet::bullet()
{

}
void bullet::set_pos(int x, int y,int rect,int row)
{
    px=x;
    py=y;
    this->rect=rect;
    this->row=row;
    bullet_pic->move(px,py);

}

peabullet::peabullet()
{
    damage=20;
    speed=6;
    frag=0;

    kind=1;

    bullet_pic=new QLabel();
    bullet_img=new QImage();
    bullet_img->load(":/image/image_1/peaBullet.png");

    bullet_pic->setPixmap(QPixmap::fromImage(*bullet_img));
}
int peabullet::update_state(vector<zombies*> zombies)
{
    // state: 0 飞行 -1 消亡 else 返回击中的僵尸序号
    if(frag>0)
    {
        frag+=1;
        if(frag==10)
            return -1;
        else
            return 0;
    }
    else
    {
        int i=0;
        for(;i<zombies.size();i++)
        {
            if(zombies[i]->row==row && zombies[i]->rect<=rect && zombies[i]->rect+speed>=rect )
            {
                if(zombies[i]->kind!=5)  //不是气球僵尸
                    break;
            }
        }
        if(i!=zombies.size())   // hit
        {
            frag=1;
            bullet_img->load(":/image/image_1/PeaBulletHit.png");
            bullet_pic->setPixmap(QPixmap::fromImage(*bullet_img));
            return i+1; // 变为正数
        }
        else
        {
            rect+=speed;
            px+=speed;
            bullet_pic->move(px,py);
            return 0;
        }
    }
}

cactusbullet::cactusbullet()
{
    damage=20;
    speed=6;
    kind=2;

    die=0;
    bullet_pic=new QLabel();
    bullet_img=new QImage();
    bullet_img->load(":/image/image_1/ProjectileCactus.png");

    bullet_pic->setPixmap(QPixmap::fromImage(*bullet_img));
}
int cactusbullet::update_state(vector<zombies*> zombies)
{
    // state: 0 飞行 -1 消亡 else 返回击中的僵尸序号
    int i=0;
    if(die==-1)
     {
        return -1;
    }
    for(;i<zombies.size();i++)
    {
        if(zombies[i]->row==row && zombies[i]->rect<=rect && zombies[i]->kind==3 && zombies[i]->rect+speed>=rect)
        {
            break;
        }
    }
    if(i!=zombies.size())   // hit
    {
        die=-1;
        return i+1; // 变为正数
    }
    else
    {
        rect+=speed;
        px+=speed;
        bullet_pic->move(px,py);
        return 0;
    }
}

catapultbullet::catapultbullet()
{
    damage=30;
    kind=3;
    speed=10;

    bullet_pic=new QLabel();
    bullet_img=new QImage();
    bullet_img->load(":/image/image_1/cataBullet.png");
    QPixmap tmp=QPixmap::fromImage(*bullet_img);
    tmp=tmp.scaled(tmp.width()*0.4,tmp.height()*0.4);
    bullet_pic->setPixmap(tmp);
}
int catapultbullet::update_state(vector<zombies *> zombies)
{
    // state: 0 飞行 -1 消亡
    if(rect<target)
        return -1;
    else
    {
        change_pos();
        bullet_pic->move(px,py);
        return 0;
    }
}
void catapultbullet::calculate(int target)
{
    this->target=target;
    a=4000000/((px-target)*(px-target));   // ???
    a=a/10000;
    b=-2*a*(target+px)/2;
    c=py-a*px*px-b*px;

}
void catapultbullet::change_pos()
{
    rect-=speed;
    px-=speed;
    py=(int)(a*px*px+b*px+c);

}

conflict::conflict(int a,int b,int c)
{
    pos_x=a;
    pos_y=b;
    life=15;
    kind=c;
}
