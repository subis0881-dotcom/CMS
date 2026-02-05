#include <QApplication>
#include "welcomedialog.h"
#include "databasemanager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // optional: if you are using database
    DatabaseManager::init();

    WelcomeDialog w;
    w.show();                 // ✅ NOT exec()
    return a.exec();          // ✅ keeps app running
}
