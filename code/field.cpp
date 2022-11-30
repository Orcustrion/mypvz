#include "field.h"
#include "ui_field.h"

field::field(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::field)
{
    ui->setupUi(this);
}

field::~field()
{
    delete ui;
}
