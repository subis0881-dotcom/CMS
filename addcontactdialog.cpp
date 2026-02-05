#include "addcontactdialog.h"
#include "ui_addcontactdialog.h"
#include <QMessageBox>

AddContactDialog::AddContactDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::AddContactDialog)
{
    ui->setupUi(this);
}

AddContactDialog::~AddContactDialog()
{
    delete ui;
}

QString AddContactDialog::name() const { return ui->nameLineEdit->text(); }
QString AddContactDialog::phone() const { return ui->phoneLineEdit->text(); }
QString AddContactDialog::email() const { return ui->emailLineEdit->text(); }
QString AddContactDialog::address() const { return ui->addressLineEdit->text(); }

void AddContactDialog::setValues(const QString &n, const QString &p,
                                 const QString &e, const QString &a)
{
    ui->nameLineEdit->setText(n);
    ui->phoneLineEdit->setText(p);
    ui->emailLineEdit->setText(e);
    ui->addressLineEdit->setText(a);
}

void AddContactDialog::on_saveBtn_clicked()
{
    if (name().isEmpty()) {
        QMessageBox::warning(this, "Error", "Name required");
        return;
    }
    accept();
}

void AddContactDialog::on_cancelBtn_clicked()
{
    reject();
}
