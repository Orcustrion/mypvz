#ifndef DIALOG_H
#define DIALOG_H

#include <QWidget>
#include <QString>
#include <QPushButton>
namespace Ui {
class dialog;
}

class dialog : public QWidget
{
    Q_OBJECT

public:
    explicit dialog(QWidget *parent = nullptr);
    ~dialog();
    QString question;
    QPushButton * left;
    QPushButton * right;

private:
    Ui::dialog *ui;
};

#endif // DIALOG_H
