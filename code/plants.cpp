#include "plants.h"

plants::plants()
{

}
void plants::plant_init(int a,int b,int c)
{
    hitpoint =a;
    planting_cd = b;
    cost = c;

}
void plants::set_plant_info(int a, int b, int c)
{
    px=a;
    py=b;

    plant_flame=c;
}
void plants::display()
{

    plant_movie->start();
    plant_gif->show();
}



sunflower::sunflower()  // 向日葵 植物id：1
{
    kind=1;
    signal=0;
    state=0;
    skill_cd=320;   // 阳光生产cd

    plant_gif = new QLabel() ;
    plant_movie = new QMovie(":/image/image_1/1.gif");
    plant_gif -> setMovie(plant_movie);

}
int sunflower::update_state(vector<zombies*> zombies)
{
    // state: -1 死亡 0 常态 1 生产阳光
    if(hitpoint<=0)
        state=-1;
    else
    {
        signal+=1;
        if(signal==skill_cd)
        {
            signal=0;
            state=1;
        }
        else
            state=0;
    }
    return state;
}

peashooter::peashooter()
{
    kind=2;
    signal=0;
    state=0;
    atkcd=70;

    plant_gif = new QLabel() ;
    plant_movie = new QMovie(":/image/image_1/2.gif");
    plant_gif -> setMovie(plant_movie);

}
int peashooter::update_state(vector<zombies*> zombies)
{
    // state: -1 死亡 0 常态 1 射击
    if(hitpoint<=0)
        state=-1;
    else
    {
        if(signal<atkcd)
            signal+=1;
        int i=0;
        for(;i<zombies.size();i++)
        {
            if(zombies[i]->row==py && zombies[i]->kind!=3)
            {
                if(signal>=atkcd)
                {

                    signal=0;
                    state=1;
                }
                else
                    state=0;
                break;
            }
        }
        if(i==zombies.size())
            state=0;
    }
    return state;
}

wallnut::wallnut()
{
    kind=3;
    state=0;
    signal=0;

    plant_gif = new QLabel() ;
    plant_movie = new QMovie(":/image/image_1/3.gif");
    plant_gif -> setMovie(plant_movie);
}
int wallnut::update_state(vector<zombies*> zombies)
{
    if(hitpoint<0)
        return -1;
    else
    {
        if(state==0)
        {
            if(hitpoint<=800)
            {
                state=1;
                plant_movie = new QMovie(":/image/image_1/3_1.gif");
                plant_gif -> setMovie(plant_movie);
                plant_movie->start();
            }
        }
        else if(state==1)
        {
            if(hitpoint<=400)
            {
                state=2;
                plant_movie = new QMovie(":/image/image_1/3_2.gif");
                plant_gif -> setMovie(plant_movie);
                plant_movie->start();
            }
        }
        return 0;
    }
}

chomper::chomper()
{
    kind=4;
    desitecd=1500;
    state=0;
    atkanime=30;

    plant_gif = new QLabel() ;
    plant_movie = new QMovie(":/image/image_1/4.gif");
    plant_gif -> setMovie(plant_movie);
}
int chomper::update_state(vector<zombies*> zombies)
{
    // state: -1 死亡 0 正常 1 进攻动画 2 进攻结束 3 消化时间
    if(hitpoint<=0)
        state=-1;
    else
    {
        if(signal<desitecd && state==3)
        {
            signal+=1;
            if(signal>=desitecd)  // 消化结束
            {
                plant_movie = new QMovie(":/image/image_1/4.gif");
                plant_gif -> setMovie(plant_movie);
                plant_movie->start();
                state=0;
                signal=0;
            }
            else
                state=3;
        }
        else if(signal<atkanime && state==1 )
        {
            signal+=1;
            if(signal>=atkanime)  // 攻击动画
            {
                plant_movie = new QMovie(":/image/image_1/4_2.gif");
                plant_gif -> setMovie(plant_movie);
                plant_movie->start();
                state=3;
                signal=0;
            }
            else if(signal == 20)
                state=2;
            else
                state=1;
        }
        else if(state==2)
        {
            state=1;
        }
        else
        {
            int i=0;
            for(;i<zombies.size();i++)
            {
                if(zombies[i]->row==py && (zombies[i]->pos==px || zombies[i]->pos==px+1)
                        && zombies[i]->kind!=3)
                {
                   state=1;
                   signal=0;
                   plant_movie = new QMovie(":/image/image_1/4_1.gif");
                   plant_gif -> setMovie(plant_movie);
                   plant_movie->start();
                   break;
                }
            }
            if(i==zombies.size())
                state=0;
        }
    }
    return state;
}

spikeweed::spikeweed()
{
    kind=5;
    state=0;
    signal=0;

    atkcd=70;
    extra_range=20;
    damage=20;

    plant_gif = new QLabel() ;
    plant_movie = new QMovie(":/image/image_1/5.gif");
    plant_gif -> setMovie(plant_movie);
}
int spikeweed::update_state(vector<zombies*> zombies)
{
    int *rects_rankfield=new int[10]{10,98,183,273,344,432,509,588,675,767};  // 横坐标
    if(hitpoint<=0)
        state=-1;
    else
    {
        if(signal<atkcd)
            signal+=1;
        int i=0;
        int rec_left=rects_rankfield[px]-20,rec_right=rects_rankfield[px+1]+20;

        for(;i<zombies.size();i++)
        {
            if(zombies[i]->row==py &&
                    (zombies[i]->rect<=rec_right && zombies[i]->rect>=rec_left)
                    && zombies[i]->kind!=3)
            {
                if(signal>=atkcd)
                {
                    signal=0;
                    state=1;
                }
                else
                    state=0;
                break;
            }
        }
        if(i==zombies.size())
            state=0;
    }
    return state;
}

cherrybomb::cherrybomb()
{
    kind=6;
    signal=0;
    state=0;
    boom_pre=15;
    boom_time=30;
    damage=1000;

    plant_gif = new QLabel() ;
    plant_movie = new QMovie(":/image/image_1/6.gif");
    plant_gif -> setMovie(plant_movie);
}
int cherrybomb::update_state(vector<zombies*> zombies)
{
    // 0 准备爆炸 1 爆炸瞬间 2 爆炸动画 -1 消亡
    if(state==0)
    {
        signal+=1;
        if(signal>boom_pre)
        {
            signal=0;
            state=1;
        }
    }
    else if(state==1)
    {
        // 修正动画位置
        plant_movie = new QMovie(":/image/image_1/6_1.gif");
        plant_gif -> setMovie(plant_movie);
        plant_gif->setFixedSize(200,200);
        plant_gif->move(plant_gif->x()-30,plant_gif->y()-30);
        plant_movie->start();
        state=2;
    }
    else if(state==2)
    {
        signal+=1;
        if(signal>boom_time)
        {
            signal=0;
            state=-1;
        }
    }
    return state;
}

snowpea::snowpea()
{
    kind=7;
    plant_gif = new QLabel() ;
    plant_movie = new QMovie(":/image/image_1/7.gif");
    plant_gif -> setMovie(plant_movie);
}
int snowpea::update_state(vector<zombies*> zombies)
{
    return 0;
}

cactus::cactus()
{
    kind=8;
    signal=0;
    state=0;
    atkcd=70;

    plant_gif = new QLabel() ;
    plant_movie = new QMovie(":/image/image_1/8.gif");
    plant_gif -> setMovie(plant_movie);
}
int cactus::update_state(vector<zombies*> zombies)
{
    // state: -1 死亡 0 常态 1 射击
    if(hitpoint<=0)
        state=-1;
    else
    {
        if(signal<atkcd)
            signal+=1;
        int i=0;
        for(;i<zombies.size();i++)
        {
            if(zombies[i]->row==py && zombies[i]->kind==3)
            {
                if(signal>=atkcd)
                {

                    signal=0;
                    state=1;
                }
                else
                    state=0;
                break;
            }
        }
        if(i==zombies.size())
            state=0;
    }
    return state;
}

