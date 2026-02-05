#include "databasemanager.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

bool DatabaseManager::init()
{
    qDebug() << "Available drivers:" << QSqlDatabase::drivers();

    // Use ODBC + DSN
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("cms_dsn");   // DSN name exactly
    db.setUserName("root");
    db.setPassword("");             // XAMPP default is empty

    if (!db.open()) {
        qDebug() << "DB open error:" << db.lastError().text();
        return false;
    }

    // Select database (important!)
    QSqlQuery q;
    if (!q.exec("USE cms_db")) {
        qDebug() << "USE DB error:" << q.lastError().text();
        return false;
    }

    return true;
}
