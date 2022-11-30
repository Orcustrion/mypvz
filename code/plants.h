#ifndef PLANTS_H
#define PLANTS_H


#include "assist.h"
#include "zombies.h"
#include <QLabel>
#include <QMovie>
#include <vector>
using namespace std;

// plants 作为父类 作为抽象类
class plants
{
public:
    plants();

    // 植物信息
    int hitpoint;        // 耐久
    int planting_cd;     // 种植cd
    int cost;            // 阳光花费
    int kind;            // 种类标志


    // 公开信息
    int plant_flame;      // 种植时间
    int px;               // 位置
    int py;

    int signal;           // 联系程序主体 消息变量
    int state;
    QLabel * plant_gif;  // 植物动图
    QMovie * plant_movie;

    // 控制函数
    void plant_init(int,int,int);
    void set_plant_info(int,int,int);
    void display();


    virtual int update_state(vector<zombies*> zombies)=0;  // 自主更新

protected:



};

class sunflower:public plants      // 向日葵
{
public:
    sunflower();
    int update_state(vector<zombies*> zombies);
private:

    int skill_cd;         // 产出阳光cd


};

class peashooter:public plants     // 豌豆射手
{
public:
    peashooter();
    int update_state(vector<zombies*> zombies);
private:
    int atkcd;                     // 射击cd
};
class wallnut:public plants        // 坚果
{
public:
    wallnut();
    int update_state(vector<zombies*> zombies);
};
class chomper:public plants        // 食人花
{
public:
    chomper();
    int update_state(vector<zombies*> zombies);
private:
    int desitecd;
    int atkanime;

};
class spikeweed:public plants      // 地刺
{
public:
    spikeweed();
    int damage;
    int update_state(vector<zombies*> zombies);
private:
    int atkcd;
    int extra_range;


};
class cherrybomb:public plants     // 樱桃炸弹
{
public:
    cherrybomb();
    int boom_pre;
    int boom_time;
    int damage;
    int update_state(vector<zombies*> zombies);
};
class snowpea:public plants        // 寒冰射手
{
public:
    snowpea();
    int update_state(vector<zombies*> zombies);
};
class cactus:public plants         // 仙人掌
{
public:
    cactus();
    int update_state(vector<zombies*> zombies);
private:
    int atkcd;
};

#endif // PLANTS_H
