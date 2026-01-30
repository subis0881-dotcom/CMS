#include "addcontactdialog.h"
#include "ui_addcontactdialog.h"

#include <QMessageBox>

AddContactDialog::AddContactDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddContactDialog)
{
    ui->setupUi(this);
}

AddContactDialog::~AddContactDialog()
{
    delete ui;
}

QString AddContactDialog::name() const    { return ui->nameLineEdit->text().trimmed(); }
QString AddContactDialog::phone() const   { return ui->phoneLineEdit->text().trimmed(); }
QString AddContactDialog::email() const   { return ui->emailLineEdit->text().trimmed(); }
QString AddContactDialog::address() const { return ui->addressLineEdit->text().trimmed(); }

void AddContactDialog::setData(const QString &name,
                               const QString &phone,
                               const QString &email,
                               const QString &address)
{
    ui->nameLineEdit->setText(name);
    ui->phoneLineEdit->setText(phone);
    ui->emailLineEdit->setText(email);
    ui->addressLineEdit->setText(address);
}

void AddContactDialog::on_saveBtn_clicked()
{
    if (name().isEmpty()) {
        QMessageBox::warning(this, "Validation", "Name is required.");
        return;
    }
    accept();
}

void AddContactDialog::on_cancelBtn_clicked()
{
    reject();
}

