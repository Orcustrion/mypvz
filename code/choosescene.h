#ifndef CHOOSESCENE_H
#define CHOOSESCENE_H

#include <QWidget>
#include <QPushButton>
#include <coloregg.h>
#include <game_control.h>


namespace Ui {
class choosescene;
}

class choosescene : public QWidget
{
    Q_OBJECT

public:
    explicit choosescene(QWidget *parent = nullptr);
    void ChooseSceneRunning();
    ~choosescene();

    coloregg * Coloregg = NULL;

    QPushButton * flowerbutton_end= NULL;
    QPushButton * flowerbutton_help= NULL;
    QPushButton * flowerbutton_options= NULL;
    QPushButton * gravebutton_1 =NULL;
    QPushButton * gravebutton_2 =NULL;
    QPushButton * gravebutton_3 =NULL;
    QPushButton * gravebutton_4 =NULL;
    QPushButton * record_button =NULL;

    game_control * Game_control=NULL;

private:
    Ui::choosescene *ui;
};

#endif // CHOOSESCENE_H
