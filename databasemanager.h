#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QString>

class DatabaseManager
{
public:
    static bool init();  // open DB + create tables
};

#endif // DATABASEMANAGER_H

