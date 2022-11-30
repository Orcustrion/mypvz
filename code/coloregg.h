#ifndef COLOREGG_H
#define COLOREGG_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

namespace Ui {
class coloregg;
}

class coloregg : public QWidget
{
    Q_OBJECT

public:
    explicit coloregg(QWidget *parent = nullptr);
    ~coloregg();

    QPushButton * back_to_menu = NULL;
    QLabel * back_to_menu_label = NULL;



private:
    Ui::coloregg *ui;
signals:
    void signal_back();
};

#endif // COLOREGG_H
