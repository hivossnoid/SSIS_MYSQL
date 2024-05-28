#include <QtSql>
#include <QtDebug>
#include <QApplication>
#include <QtSql>
#include <QtDebug>
#include <QApplication>

bool connectToDB()
{
    QCoreApplication::addLibraryPath("C:/Qt/6.7.0/mingw_64/plugins");

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");

    if (!db.isValid()) {
        qDebug() << "FAILED TO LOAD DRIVER" << QSqlDatabase::drivers();
        return false;
    }

    db.setHostName("localhost");
    db.setDatabaseName("SSIS");
    db.setUserName("root");
    db.setPassword("Antarisks159357");

    if (!db.open()) {
        qDebug() << "Database not connected" << db.lastError();
        return false;
    } else {
        qDebug() << "Database connected";
        return true;
    }
}


