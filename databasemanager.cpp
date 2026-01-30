#include "databasemanager.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardPaths>
#include <QDir>
#include <QDebug>

bool DatabaseManager::init()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    QString dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(dir);

    QString dbPath = dir + "/contacts.db";
    db.setDatabaseName(dbPath);

    qDebug() << "DB PATH =" << dbPath;

    if (!db.open()) {
        qDebug() << "DB open error:" << db.lastError().text();
        return false;
    }

    QSqlQuery query;

    if (!query.exec(
            "CREATE TABLE IF NOT EXISTS users ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "username TEXT UNIQUE NOT NULL,"
            "password TEXT NOT NULL)"
            )) {
        qDebug() << "Create users error:" << query.lastError().text();
        return false;
    }

    if (!query.exec(
            "CREATE TABLE IF NOT EXISTS contacts ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "name TEXT NOT NULL,"
            "phone TEXT,"
            "email TEXT,"
            "address TEXT)"
            )) {
        qDebug() << "Create contacts error:" << query.lastError().text();
        return false;
    }

    return true;
}
