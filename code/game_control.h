#ifndef GAME_CONTROL_H
#define GAME_CONTROL_H

#include <QWidget>
#include <ctime>
#include <QPushButton>
#include <QMouseEvent>
#include <QMovie>
#include <QLabel>
#include <vector>
#include <QFile>
#include <QTextStream>
#include <QTimer>
#include <QElapsedTimer>
#include <cstdlib>
#include <QVector>
#include <QPainter>

using namespace std;
#include "plants.h"
#include "zombies.h"
#include "assist.h"

namespace Ui {
class game_control;
}

class game_control : public QWidget
{
    Q_OBJECT

public:
    explicit game_control(QWidget *parent = nullptr);
    ~game_control();

    // 游戏运行函数
    void game_control_running();

    // 场景重载事件
    void paintEvent(QPaintEvent *);

    // 游戏控制基础信息
    int get_start_time();
    int get_mode();
    int get_mapkind();

    // 游戏基本控制
    void initial_game_controler(int,int,int);
    void window_setting();

    // 选植物函数
    void select_plants();
    void is_planting_change(int);

    // 鼠标事件
    int temp_x;  // 暂存鼠标坐标
    int temp_y;
    void mousePressEvent(QMouseEvent * );  // 鼠标事件
    void mouseMoveEvent(QMouseEvent * );
    void mouseReleaseEvent(QMouseEvent * );

    // 场地相关信息
    int* where_in_feild(int,int); // 坐标转格子
    void read_field_info();
    QLabel * wave_view;       // 波数显示
    QPushButton * pause;
    int is_running;


    // 植物相关函数
    void planting(int,int,int);    // 种植植物函数
    void read_plants_info();       // 读取植物信息文件
    void del_plants(int,int);      // 植物消亡
    void (game_control::*plants_functions[9])(int,int);      // 植物相应函数指针数组

    // 植物函数定义 参数：状态更新后的返回值，容器中位置
    void plant_1(int,int);      // 向日葵
    void plant_2(int,int);
    void plant_3(int,int);
    void plant_4(int,int);
    void plant_5(int,int);
    void plant_6(int,int);
    void plant_7(int,int);
    void plant_8(int,int);

    // 僵尸相关函数
    void read_zombies_info();
    void control_newzombie();           // 综合控制
    void add_newzombie(int,int );       // 增加新的僵尸
    void del_zombie(int);                  // 删除僵尸
    void handle_catapult(int,int);
    int intervel;

    // 子弹
    void add_newbullet(int,int,int);
    void del_bullet(int);
    vector<conflict> conflicts;

    // 阳光
    int sunshine;                  // 阳光量
    QLabel * sunshine_view ;       // 阳光显示
    vector<SunShine *> sunshine_vector; // 阳光保存容器
    void produce_SunShine(int,int);            // 阳光生产,参数：方式、容器位置

    // 铲子
    shovel * Shovel;

    // 手套
    glove * Glove;

    // buff
    void get_buff(int);
    QPushButton** buff_buttons; // 对应的按钮数组
    int shovel_change;
    int glove_change;
    int sunshine_change;

    // 游戏相关结算
    void start_anime();     // 开场动画
    void main_circle();     // 游戏主循环 监听定时器信号
    void game_end(int);        // 游戏结束

    void handle_attack(int);   // 处理僵尸进攻
    void handle_bullet(int,int);  // 处理子弹碰撞


private:
    Ui::game_control *ui;

    // 游戏控制基础信息
    long start_time;  // 开始时间记录
    int mode;        // 模式记录
    int level;       // 关卡记录
    int mapkind;     // 地图种类记录

    int inter;
    int sun_inter;


    // 游戏操作控制信息
    bool is_planting;  // 事件：正在种植植物
    int is_planting_kind;
    bool is_shoveling; // 事件：正在调用铲子
    int is_gloving;   // 事件：正在调用手套
    int is_gloving_seq;


    // 植物选择槽相关定义
    int selection_number;  // 可携带的植物个数
    int* selection_kinds;  // 携带植物的种类
    QPushButton** seed_buttons; // 对应的按钮数组

    // 场地相关信息
    int rownum;            // 排数
    int** plants_field;    // 5或6排 9列 记录植物种类
    int** zombies_field;   // 记录僵尸种类
    int* rects_rowfield;   // 行边界信息
    int* rects_rankfield;  // 列边界信息

    // 植物相关信息
    vector<plants * > plants_vector; // 植物类存储容器
    int plant_kinds;
    int ** plants_info;    // 植物初始化信息
    int * planting_cd;     // 植物cd记录
    QLabel** planting_b;


    // 僵尸相关信息
    vector<zombies * > zombies_vector;
    vector<zombies *> wave_vector;
    int zombie_kinds;
    int ** zombies_info;
    int wave;           // 僵尸波数
    int * waving;         // 僵尸潮中

    // 子弹相关信息
    vector<bullet * > bullets_vector;


    // 定时器
    QTimer* game_timer;
    int sum_flame;  // 帧总数



};

#endif // GAME_CONTROL_H
