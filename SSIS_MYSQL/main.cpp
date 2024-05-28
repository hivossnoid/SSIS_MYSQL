#include "mainwindow.h"
#include <QtSql>
#include <QtDebug>
#include <QApplication>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.show();
    return a.exec();
}
