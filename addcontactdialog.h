#ifndef ADDCONTACTDIALOG_H
#define ADDCONTACTDIALOG_H

#include <QDialog>

namespace Ui {
class AddContactDialog;
}

class AddContactDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddContactDialog(QWidget *parent = nullptr);
    ~AddContactDialog();

    QString name() const;
    QString phone() const;
    QString email() const;
    QString address() const;

    void setData(const QString &name,
                 const QString &phone,
                 const QString &email,
                 const QString &address);

private slots:
    void on_saveBtn_clicked();
    void on_cancelBtn_clicked();

private:
    Ui::AddContactDialog *ui;
};

#endif // ADDCONTACTDIALOG_H


