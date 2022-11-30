#ifndef ZOMBIES_H
#define ZOMBIES_H

#include <QLabel>
#include <QMovie>
#include <QDebug>

//僵尸序号 0 普通 1 路障 2 铁桶 3 气球 4 投石车
class zombies
{

public:
    int kind;           // 种类
    int lifepoint;      // 生命值
    int atk;            // 攻击
    int speed;          // 速度

    //边界和行数唯一确定位置
    int rect;           // 边界（坐标）
    int row;            // 行数
    int pos;            // 格子

    int add_flame;      // 出生时间
    int px;             // 动画位置（坐标）
    int py;
    int die_flame;      // 死亡动画帧


    int sign;           // 攻击信号
    int atkcd;          // 攻击cd
    int movcd;          // 移动cd

    QLabel * zombie_gif;  // 僵尸动图
    QMovie * zombie_movie;
    QImage * zombie_img;  // 僵尸图片

    zombies();

    void zombie_init(int,int,int);
    void set_zombie_info(int,int);
    void display();
    void change_pos();
    // 僵尸格子：field[pos][row]

    virtual int update_state(int** )=0;
    virtual int zombie_move()=0;
    virtual int zombie_attack()=0;
};

class normal_zombie:public zombies
{
public:
    normal_zombie();
    int update_state(int** );
    int zombie_move();
    int zombie_attack();
};

class conehead_zombie:public zombies
{
public:
    int orn;
    conehead_zombie();
    int update_state(int** );
    int zombie_move();
    int zombie_attack();
};
class buckethead_zombie:public zombies
{
public:
    int orn;
    buckethead_zombie();
    int update_state(int** );
    int zombie_move();
    int zombie_attack();
};
class balloon_zombie:public zombies
{
public:
    int move_sign;
    balloon_zombie();
    int update_state(int** );
    int zombie_move();
    int zombie_attack();
};
class catapult_zombie:public zombies
{
public:
    int bullets;
    catapult_zombie();
    int update_state(int** );
    int zombie_move();
    int zombie_attack();
};

#endif // ZOMBIES_H
