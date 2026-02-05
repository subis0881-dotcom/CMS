#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addcontactdialog.h"
#include "logindialog.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupTable();
    loadContacts();

    connect(ui->searchLineEdit, &QLineEdit::textChanged,
            this, &MainWindow::on_searchLineEdit_textChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupTable()
{
    ui->contactsTable->setColumnCount(5);
    ui->contactsTable->setHorizontalHeaderLabels(
        {"ID", "Name", "Phone", "Email", "Address"}
        );

    ui->contactsTable->setColumnHidden(0, true); // hide ID
    ui->contactsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->contactsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->contactsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->contactsTable->horizontalHeader()->setStretchLastSection(true);
}

void MainWindow::loadContacts(const QString &filter)
{
    ui->contactsTable->setRowCount(0);

    QSqlQuery query;
    QString sql = "SELECT id, name, phone, email, address FROM contacts";

    if (!filter.trimmed().isEmpty()) {
        sql += " WHERE name LIKE ? OR phone LIKE ? OR email LIKE ? OR address LIKE ?";
    }

    query.prepare(sql);

    if (!filter.trimmed().isEmpty()) {
        QString f = "%" + filter + "%";
        query.addBindValue(f);
        query.addBindValue(f);
        query.addBindValue(f);
        query.addBindValue(f);
    }

    if (!query.exec()) {
        QMessageBox::critical(this, "DB Error", query.lastError().text());
        return;
    }

    int row = 0;
    while (query.next()) {
        ui->contactsTable->insertRow(row);
        for (int col = 0; col < 5; col++) {
            ui->contactsTable->setItem(
                row, col,
                new QTableWidgetItem(query.value(col).toString())
                );
        }
        row++;
    }
}

int MainWindow::selectedId() const
{
    int row = ui->contactsTable->currentRow();
    if (row < 0) return -1;
    return ui->contactsTable->item(row, 0)->text().toInt();
}

// ---------- ADD ----------
void MainWindow::on_addBtn_clicked()
{
    AddContactDialog dlg(this);

    if (dlg.exec() == QDialog::Accepted) {
        QSqlQuery q;
        q.prepare(
            "INSERT INTO contacts(name, phone, email, address) "
            "VALUES(?,?,?,?)"
            );
        q.addBindValue(dlg.name());
        q.addBindValue(dlg.phone());
        q.addBindValue(dlg.email());
        q.addBindValue(dlg.address());

        if (!q.exec()) {
            QMessageBox::critical(this, "DB Error", q.lastError().text());
            return;
        }

        loadContacts(); // 🔥 THIS WAS MISSING BEFORE
    }
}

// ---------- EDIT ----------
void MainWindow::on_editBtn_clicked()
{
    int id = selectedId();
    if (id < 0) {
        QMessageBox::warning(this, "Edit", "Select a row first.");
        return;
    }

    int r = ui->contactsTable->currentRow();

    AddContactDialog dlg(this);
    dlg.setValues(
        ui->contactsTable->item(r,1)->text(),
        ui->contactsTable->item(r,2)->text(),
        ui->contactsTable->item(r,3)->text(),
        ui->contactsTable->item(r,4)->text()
        );

    if (dlg.exec() == QDialog::Accepted) {
        QSqlQuery q;
        q.prepare(
            "UPDATE contacts SET name=?, phone=?, email=?, address=? WHERE id=?"
            );
        q.addBindValue(dlg.name());
        q.addBindValue(dlg.phone());
        q.addBindValue(dlg.email());
        q.addBindValue(dlg.address());
        q.addBindValue(id);

        if (!q.exec()) {
            QMessageBox::critical(this, "DB Error", q.lastError().text());
            return;
        }

        loadContacts();
    }
}

// ---------- DELETE ----------
void MainWindow::on_deleteBtn_clicked()
{
    int id = selectedId();
    if (id < 0) {
        QMessageBox::warning(this, "Delete", "Select a row first.");
        return;
    }

    if (QMessageBox::question(this, "Delete", "Delete contact?")
        != QMessageBox::Yes)
        return;

    QSqlQuery q;
    q.prepare("DELETE FROM contacts WHERE id=?");
    q.addBindValue(id);

    if (!q.exec()) {
        QMessageBox::critical(this, "DB Error", q.lastError().text());
        return;
    }

    loadContacts();
}

// ---------- SEARCH ----------
void MainWindow::on_searchLineEdit_textChanged(const QString &text)
{
    loadContacts(text);
}

// ---------- REFRESH ----------
void MainWindow::on_refreshBtn_clicked()
{
    loadContacts();
}

// ---------- CLEAR ----------
void MainWindow::on_clearBtn_clicked()
{
    ui->searchLineEdit->clear();
    loadContacts();
}

// ---------- LOGOUT ----------
void MainWindow::on_logoutBtn_clicked()
{
    hide();
    LoginDialog login;
    if (login.exec() == QDialog::Accepted) {
        show();
        loadContacts();
    } else {
        qApp->quit();
    }
}
