#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    /*choosescene Choosescene;
    Choosescene.show();*/
    return a.exec();
}
