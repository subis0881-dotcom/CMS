#include <QApplication>

#include "databasemanager.h"
#include "logindialog.h"
#include "mainwindow.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (!DatabaseManager::init()) {
        return 1;
    }

    LoginDialog login;
    if (login.exec() == QDialog::Accepted) {
        MainWindow w;
        w.show();
        return a.exec();
    }
    return 0;
}


