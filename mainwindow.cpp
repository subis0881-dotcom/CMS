#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addcontactdialog.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QTableWidgetItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupTable();
    loadContacts();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupTable()
{
    // We will store ID hidden inside column 0 item using Qt::UserRole
    ui->contactsTable->setColumnCount(4);
    ui->contactsTable->setHorizontalHeaderLabels({"Name", "Phone", "Email", "Address"});

    ui->contactsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->contactsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->contactsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::loadContacts(const QString &filterText)
{
    ui->contactsTable->setRowCount(0);

    QSqlQuery query;

    if (filterText.trimmed().isEmpty()) {
        query.prepare("SELECT id, name, phone, email, address FROM contacts ORDER BY id DESC");
    } else {
        QString pattern = "%" + filterText.trimmed() + "%";
        query.prepare(
            "SELECT id, name, phone, email, address FROM contacts "
            "WHERE name LIKE ? OR phone LIKE ? OR email LIKE ? OR address LIKE ? "
            "ORDER BY id DESC"
            );
        query.addBindValue(pattern);
        query.addBindValue(pattern);
        query.addBindValue(pattern);
        query.addBindValue(pattern);
    }

    if (!query.exec()) {
        QMessageBox::critical(this, "DB Error", "Failed to load contacts:\n" + query.lastError().text());
        return;
    }

    int row = 0;
    while (query.next()) {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        QString phone = query.value(2).toString();
        QString email = query.value(3).toString();
        QString address = query.value(4).toString();

        ui->contactsTable->insertRow(row);

        // Column 0: Name (store id in UserRole)
        QTableWidgetItem *nameItem = new QTableWidgetItem(name);
        nameItem->setData(Qt::UserRole, id);
        ui->contactsTable->setItem(row, 0, nameItem);

        ui->contactsTable->setItem(row, 1, new QTableWidgetItem(phone));
        ui->contactsTable->setItem(row, 2, new QTableWidgetItem(email));
        ui->contactsTable->setItem(row, 3, new QTableWidgetItem(address));

        row++;
    }
}

int MainWindow::selectedContactId() const
{
    int row = ui->contactsTable->currentRow();
    if (row < 0) return -1;

    QTableWidgetItem *item = ui->contactsTable->item(row, 0);
    if (!item) return -1;

    return item->data(Qt::UserRole).toInt();
}

/* ---------- BUTTONS ---------- */

void MainWindow::on_refreshBtn_clicked()
{
    loadContacts(ui->searchLineEdit->text().trimmed());
}

void MainWindow::on_clearBtn_clicked()
{
    ui->searchLineEdit->clear();
    loadContacts();
}

void MainWindow::on_searchLineEdit_textChanged(const QString &text)
{
    loadContacts(text);
}

void MainWindow::on_addBtn_clicked()
{
    AddContactDialog dialog(this);
    if (dialog.exec() != QDialog::Accepted)
        return;

    QSqlQuery query;
    query.prepare("INSERT INTO contacts (name, phone, email, address) VALUES (?, ?, ?, ?)");
    query.addBindValue(dialog.name());
    query.addBindValue(dialog.phone());
    query.addBindValue(dialog.email());
    query.addBindValue(dialog.address());

    if (!query.exec()) {
        QMessageBox::critical(this, "DB Error", "Failed to add contact:\n" + query.lastError().text());
        return;
    }

    loadContacts(ui->searchLineEdit->text().trimmed());
}

void MainWindow::on_deleteBtn_clicked()
{
    int id = selectedContactId();
    if (id < 0) {
        QMessageBox::warning(this, "Delete", "Select a contact row first.");
        return;
    }

    if (QMessageBox::question(this, "Delete", "Delete this contact?") != QMessageBox::Yes)
        return;

    QSqlQuery query;
    query.prepare("DELETE FROM contacts WHERE id = ?");
    query.addBindValue(id);

    if (!query.exec()) {
        QMessageBox::critical(this, "DB Error", "Failed to delete contact:\n" + query.lastError().text());
        return;
    }

    loadContacts(ui->searchLineEdit->text().trimmed());
}

void MainWindow::on_editBtn_clicked()
{
    int id = selectedContactId();
    if (id < 0) {
        QMessageBox::warning(this, "Edit", "Select a contact row first.");
        return;
    }

    int row = ui->contactsTable->currentRow();
    QString oldName = ui->contactsTable->item(row, 0)->text();
    QString oldPhone = ui->contactsTable->item(row, 1)->text();
    QString oldEmail = ui->contactsTable->item(row, 2)->text();
    QString oldAddress = ui->contactsTable->item(row, 3)->text();

    AddContactDialog dialog(this);
    dialog.setWindowTitle("Edit Contact");

    // If your AddContactDialog has setters, use them.
    // If not, we will still edit using DB by opening dialog empty (you can type again).
    // BEST: add setters later.
    // For now, user will re-enter values.

    if (dialog.exec() != QDialog::Accepted)
        return;

    // If user leaves fields empty, keep old values (nice behavior)
    QString newName = dialog.name().isEmpty() ? oldName : dialog.name();
    QString newPhone = dialog.phone().isEmpty() ? oldPhone : dialog.phone();
    QString newEmail = dialog.email().isEmpty() ? oldEmail : dialog.email();
    QString newAddress = dialog.address().isEmpty() ? oldAddress : dialog.address();

    QSqlQuery query;
    query.prepare("UPDATE contacts SET name=?, phone=?, email=?, address=? WHERE id=?");
    query.addBindValue(newName);
    query.addBindValue(newPhone);
    query.addBindValue(newEmail);
    query.addBindValue(newAddress);
    query.addBindValue(id);

    if (!query.exec()) {
        QMessageBox::critical(this, "DB Error", "Failed to update contact:\n" + query.lastError().text());
        return;
    }

    loadContacts(ui->searchLineEdit->text().trimmed());
}

void MainWindow::on_logoutBtn_clicked()
{
    close();
}
