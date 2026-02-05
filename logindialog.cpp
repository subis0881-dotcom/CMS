#include "logindialog.h"
#include "ui_logindialog.h"
#include "registerdialog.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}
void LoginDialog::on_cancelBtn_clicked()
{
    reject();
}

void LoginDialog::on_registerBtn_clicked()
{
    RegisterDialog reg(this);
    reg.exec();
}

void LoginDialog::on_loginBtn_clicked()
{
    QString username = ui->usernameLineEdit->text().trimmed();
    QString password = ui->passwordLineEdit->text().trimmed();

    QSqlQuery query;
    query.prepare("SELECT 1 FROM users WHERE username = ? AND password = ?");
    query.addBindValue(username);
    query.addBindValue(password);

    if (!query.exec()) {
        QMessageBox::critical(this, "DB Error", query.lastError().text());
        return;
    }

    if (query.next()) {
        accept();
    } else {
        QMessageBox::warning(this, "Login Failed", "Invalid username or password.");
    }
}
