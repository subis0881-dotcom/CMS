#include "registerdialog.h"
#include "ui_registerdialog.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>


RegisterDialog::RegisterDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::on_cancelBtn_clicked()
{
    reject();
}
void RegisterDialog::on_registerBtn_clicked()
{
    QString username = ui->usernameLineEdit_->text().trimmed();
    QString password = ui->passwordLineEdit_->text().trimmed();
    QString confirm  = ui->confirmPasswordLineEdit_->text().trimmed();

    if (username.isEmpty() || password.isEmpty() || confirm.isEmpty()) {
        QMessageBox::warning(this, "Register", "Please fill all fields.");
        return;
    }

    if (password != confirm) {
        QMessageBox::warning(this, "Register", "Passwords do not match.");
        return;
    }

    QSqlQuery query;

    // 1) check duplicate username
    query.prepare("SELECT 1 FROM users WHERE username = ?");
    query.addBindValue(username);

    if (!query.exec()) {
        QMessageBox::critical(this, "DB Error", "Check username failed:\n" + query.lastError().text());
        return;
    }
    if (query.next()) {
        QMessageBox::warning(this, "Register", "Username already exists.");
        return;
    }

    // 2) insert new user
    query.prepare("INSERT INTO users (username, password) VALUES (?, ?)");
    query.addBindValue(username);
    query.addBindValue(password);

    if (!query.exec()) {
        QMessageBox::critical(this, "DB Error", "Insert failed:\n" + query.lastError().text());
        return;
    }

    QMessageBox::information(this, "Register", "Registration successful!");
    accept();
}
