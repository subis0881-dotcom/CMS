
#include "welcomedialog.h"
#include "ui_welcomedialog.h"
#include "logindialog.h"
#include "mainwindow.h"
#include <QApplication>

WelcomeDialog::WelcomeDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::WelcomeDialog)
{
    ui->setupUi(this);
}

WelcomeDialog::~WelcomeDialog()
{
    delete ui;
}

void WelcomeDialog::on_enterBtn_clicked()
{
    LoginDialog login(this);

    if (login.exec() == QDialog::Accepted) {
        this->hide();                 // hide welcome

        MainWindow *mw = new MainWindow();
        mw->show();                   // show main window
    }
}

void WelcomeDialog::on_exitBtn_clicked()
{
    QApplication::quit();
}
