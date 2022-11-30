#include "zombies.h"

zombies::zombies()
{

}

void zombies::zombie_init(int a,int b,int c)
{
    lifepoint=a;
    atk=b;
    speed=c;

    sign=0;
    atkcd=0;
    movcd=0;
    die_flame=0;
}
void zombies::set_zombie_info(int a,int b)         // 参数：行数、时间、边界
{
    row=a;
    add_flame=b;
    rect=700;
    change_pos();
}
void zombies::display()
{
    zombie_gif->setFixedSize(166,144);

    zombie_gif->move(px,py);
    if(kind<=2)
        zombie_movie->start();
    zombie_gif->show();


}
void zombies::change_pos()
{
    int * rects_rankfield=new int[10]{10,98,183,273,344,432,509,588,675,767};  // 横坐标
    for(int i=0;i<10;i++)
        if(rect<rects_rankfield[i])
        {
            pos=i-1;
            break;
        }
}

normal_zombie::normal_zombie()
{
    kind=0;

    zombie_gif = new QLabel() ;
    zombie_movie = new QMovie(":/image/image_3/Zombie.gif");
    zombie_gif -> setMovie(zombie_movie);

}
int normal_zombie::update_state(int ** plants_field)
{
    // 返回值 -1 死亡 0 正常移动 1 进攻帧 2 进攻cd 10000 进入房子
    if(die_flame>0)
    {
        die_flame+=1;
        if(die_flame>=50)
            return -1;
        else
            return 0;
    }
    // 行为：判断血量
    else
    {
        if(lifepoint<=0)
        {
            die_flame=1;
            zombie_movie = new QMovie(":/image/image_3/ZombieDie.gif");
            zombie_gif -> setMovie(zombie_movie);
            zombie_movie->start();
            return 0;
        }

        // 行为：判断前方是否有阻挡
        if(plants_field[pos][row]==0 || plants_field[pos][row]==5 ||plants_field[pos][row]==-1) //地刺不阻挡
        {
            // 移动
            int res=zombie_move();
            change_pos();
            if(rect<10)  // 进入房子
                return 999;
            else
                return res;
        }
        else
        {
            // 进攻
            int res=zombie_attack();
            return res;
        }
    }
}
int normal_zombie::zombie_move()
{
    if(sign==1)
    {
        sign=0;
        zombie_movie = new QMovie(":/image/image_3/Zombie.gif");
        zombie_gif -> setMovie(zombie_movie);
        zombie_movie->start();
    }
    movcd+=1;
    if(movcd>=10)
    {
        movcd=0;
        rect-=speed;
        px-=speed;

        zombie_gif->move(px,py);
    }
    return 0;
}
int normal_zombie::zombie_attack()
{
    if(sign!=1)
    {
        sign=1;
        zombie_movie = new QMovie(":/image/image_3/ZombieAttack.gif");
        zombie_gif -> setMovie(zombie_movie);
        zombie_movie->start();
    }
    atkcd+=1;
    if(atkcd>=25)
    {
        // make an attack
        atkcd=0;
        return 1;
    }
    else
        // wait for an attack
        return 2;
}

conehead_zombie::conehead_zombie()
{
    kind=1;
    orn=1;

    zombie_gif = new QLabel() ;
    zombie_movie = new QMovie(":/image/image_3/ConeheadZombie.gif");
    zombie_gif -> setMovie(zombie_movie);

}
int conehead_zombie::update_state(int ** plants_field)
{
    if(die_flame>0)
    {
        die_flame+=1;
        if(die_flame>=50)
            return -1;
        else
            return 0;
    }
    // 行为：判断血量
    else
    {
        if(lifepoint<=0)
        {
            die_flame=1;
            zombie_movie = new QMovie(":/image/image_3/ZombieDie.gif");
            zombie_gif -> setMovie(zombie_movie);
            zombie_movie->start();
            return 0;
        }
        if(lifepoint<=200 && orn==1)
        {
            orn=0;
            sign=0;
            zombie_movie = new QMovie(":/image/image_3/Zombie.gif");
            zombie_gif -> setMovie(zombie_movie);
            zombie_movie->start();
        }
        // 行为：判断前方是否有阻挡
        if(plants_field[pos][row]==0 || plants_field[pos][row]==5||plants_field[pos][row]==-1) //地刺不阻挡
        {
            // 移动
            int res=zombie_move();
            change_pos();
            if(rect<10)  // 进入房子
                return 999;
            else
                return res;
        }
        else
        {
            // 进攻
            int res=zombie_attack();
            return res;
        }
    }
}
int conehead_zombie::zombie_move()
{
    if(sign==1)
    {

        sign=0;
        if(orn==1)
            zombie_movie = new QMovie(":/image/image_3/ConeheadZombie.gif");
        else
            zombie_movie = new QMovie(":/image/image_3/Zombie.gif");
        zombie_gif -> setMovie(zombie_movie);
        zombie_movie->start();
    }
    movcd+=1;
    if(movcd>=10)
    {
        movcd=0;
        rect-=speed;
        px-=speed;

        zombie_gif->move(px,py);
    }
    return 0;
}
int conehead_zombie::zombie_attack()
{
    if(sign!=1)
    {
        sign=1;
        if(orn==1)
            zombie_movie = new QMovie(":/image/image_3/ConeheadZombieAttack.gif");
        else
            zombie_movie = new QMovie(":/image/image_3/ZombieAttack.gif");
        zombie_gif -> setMovie(zombie_movie);
        zombie_movie->start();
    }
    atkcd+=1;
    if(atkcd>=25)
    {
        // make an attack
        atkcd=0;
        return 1;
    }
    else
        // wait for an attack
        return 2;
}


buckethead_zombie::buckethead_zombie()
{
    kind=2;
    orn=1;

    zombie_gif = new QLabel() ;
    zombie_movie = new QMovie(":/image/image_3/BucketheadZombie.gif");
    zombie_gif -> setMovie(zombie_movie);

}
int buckethead_zombie::update_state(int ** plants_field)
{
    if(die_flame>0)
    {
        die_flame+=1;
        if(die_flame>=50)
            return -1;
        else
            return 0;
    }
    // 行为：判断血量
    else
    {
        if(lifepoint<=0)
        {
            die_flame=1;
            zombie_movie = new QMovie(":/image/image_3/ZombieDie.gif");
            zombie_gif -> setMovie(zombie_movie);
            zombie_movie->start();
            return 0;
        }
        if(lifepoint<=200 && orn==1)
        {
            orn=0;
            sign=0;
            zombie_movie = new QMovie(":/image/image_3/Zombie.gif");
            zombie_gif -> setMovie(zombie_movie);
            zombie_movie->start();
        }
        // 行为：判断前方是否有阻挡
        if(plants_field[pos][row]==0 || plants_field[pos][row]==5||plants_field[pos][row]==-1) //地刺不阻挡
        {
            // 移动
            int res=zombie_move();
            change_pos();
            if(rect<10)  // 进入房子
                return 999;
            else
                return res;
        }
        else
        {
            // 进攻
            int res=zombie_attack();
            return res;
        }
    }
}
int buckethead_zombie::zombie_move()
{
    if(sign==1)
    {
        sign=0;
        if(orn==1)
            zombie_movie = new QMovie(":/image/image_3/BucketheadZombie.gif");
        else
            zombie_movie = new QMovie(":/image/image_3/Zombie.gif");
        zombie_gif -> setMovie(zombie_movie);
        zombie_movie->start();
    }
    movcd+=1;
    if(movcd>=10)
    {
        movcd=0;
        rect-=speed;
        px-=speed;

        zombie_gif->move(px,py);
    }
    return 0;
}
int buckethead_zombie::zombie_attack()
{
    if(sign!=1)
    {
        qDebug()<<"!!!";
        sign=1;
        if(orn==1)
            zombie_movie = new QMovie(":/image/image_3/BucketheadZombieAttack.gif");
        else
        {

            zombie_movie = new QMovie(":/image/image_3/ZombieAttack.gif");
        }
        zombie_gif -> setMovie(zombie_movie);
        zombie_movie->start();
    }
    atkcd+=1;
    if(atkcd>=25)
    {
        // make an attack
        atkcd=0;
        return 1;
    }
    else
        // wait for an attack
        return 2;
}

balloon_zombie::balloon_zombie()
{
    kind=3;
    move_sign=0;

    zombie_gif=new QLabel();
    zombie_img=new QImage();

    zombie_img->load(":/image/image_3/balloon.png");
    QPixmap tmp=QPixmap::fromImage(*zombie_img);
    tmp=tmp.scaled(tmp.width()*0.3,tmp.height()*0.3);
    zombie_gif->setPixmap(tmp);

}
int balloon_zombie::update_state(int ** plants_field)
{
    if(lifepoint<0)
        return -1;
    // 行为：判断血量
    else
    {
        // 移动
        int res=zombie_move();
        change_pos();
        if(rect<10)  // 进入房子
            return 999;
        else
            return res;
    }
}
int balloon_zombie::zombie_move()
{
    movcd+=1;
    if(movcd>=10)
    {
        movcd=0;
        rect-=speed;
        px-=speed;
        move_sign+=1;
        if(move_sign>=0)
            py+=1;
        else
            py-=1;
        if(move_sign==5 || move_sign==-6)
            move_sign=-move_sign;
        zombie_gif->move(px,py);
    }
    return 0;
}
int balloon_zombie::zombie_attack()
{
    // no attacks
    return 0;
}

catapult_zombie::catapult_zombie()
{
    kind=4;

    bullets=15;

    zombie_gif=new QLabel();
    zombie_img=new QImage();

    zombie_img->load(":/image/image_3/catapultzombie.png");
    QPixmap tmp=QPixmap::fromImage(*zombie_img);
    tmp=tmp.scaled(tmp.width()*0.5,tmp.height()*0.5);
    zombie_gif->setPixmap(tmp);

}
int catapult_zombie::update_state(int ** plants_field)
{
    // -1 消亡 0~8 attack 10 move 11 normal
    if(lifepoint<=0)
        return -1;
    // 行为：判断血量
    else
    {
        int i=0;
        for(;i<9;i++)
            if(plants_field[i][row]!=0 && plants_field[i][row]!=5 &&plants_field[i][row]!=-1)
                break;
        if(bullets>0 && i!=9 && rect<650 && rect>300) // 发射子弹
        {
            // 进攻
            int res=zombie_attack();
            if(res==1)
                return i;
            else
                return 11;
        }
        else
        {
            // 移动
            int res=zombie_move();
            change_pos();
            if(rect<10)  // 进入房子
                return 999;
            else
                return res;
        }
    }
}
int catapult_zombie::zombie_move()
{
    movcd+=1;
    if(movcd>=10)
    {
        movcd=0;
        rect-=speed;
        px-=speed;

        zombie_gif->move(px,py);
    }
    return 10;
}
int catapult_zombie::zombie_attack()
{
    atkcd+=1;
    if(atkcd>=40)
    {
        // make an attack
        atkcd=0;
        bullets-=1;
        return 1;
    }
    else
        // wait for an attack
        return 0;
}
