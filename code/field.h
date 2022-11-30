#ifndef FIELD_H
#define FIELD_H

#include <QWidget>

namespace Ui {
class field;
}

class field : public QWidget
{
    Q_OBJECT

public:
    explicit field(QWidget *parent = nullptr);
    ~field();

private:
    Ui::field *ui;
};

#endif // FIELD_H
