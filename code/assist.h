#ifndef ASSIST_H
#define ASSIST_H

#include <QPushButton>
#include <QLabel>
#include <QMovie>
#include <QVBoxLayout>
#include <vector>
#include "zombies.h"
using namespace std;

class SunShine
{
private:
    int sunshine_kind; // 自然阳光1 生产阳光2

    int px;            // 位置(图片坐标)
    int py;
    int birth;         // 出生时间
    int exist_time;    // 存在时间


public:
   QVBoxLayout * layout;
   QLabel * sun_gif;
   QMovie * sun_movie;
   QPushButton * instructor;
   int sun_number;    // 阳光数量
   SunShine();
   SunShine(int px,int py,int kind,int birth);

   void display();
   int update_state(int);  // 阳光状态： 1 消亡 2 移动
   int get_number();

};

class shovel          // 铲子
{
private:


public:
    int px;
    int py;
    QPushButton * shovel_button;
    shovel();
};

class glove
{
public:
    int px;
    int py;
    QPushButton * glove_button;
    glove();
};

class bullet          // 子弹
{
private:

public:
    bullet();
    int damage;       // 伤害
    int speed;        // 飞行速度

    QLabel * bullet_pic;  // 子弹图片
    QImage * bullet_img;

    int px;           // 子弹图片
    int py;
    int row;          // 子弹纵坐标
    int rect;         // 击打边界
    int kind;

    void set_pos(int,int,int,int);
    virtual int update_state(vector<zombies*> zombies)=0;



};
class peabullet:public bullet
{
public:
    int frag;             // 碎裂时间
    peabullet();
    int update_state(vector<zombies*> zombies);
};

class cactusbullet:public bullet
{
public:
    int die;
    cactusbullet();
    int update_state(vector<zombies*> zombies);
};

class catapultbullet:public bullet
{
public:
    int target;
    int pos;

    double a;    // 抛物线参数
    double b;
    double c;
    catapultbullet();
    int update_state(vector<zombies*> zombies);
    void calculate(int);
    void change_pos();
};





class conflict
{
public:
    int pos_x;  // 横坐标
    int pos_y;  // 纵坐标
    int life;   // 展示剩余时间
    int kind;   // 种类：1 僵尸受伤 2 植物受伤
    conflict(int a,int b,int c);
};

#endif // ASSIST_H
