#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_logoutBtn_clicked();
    void on_refreshBtn_clicked();
    void on_addBtn_clicked();
    void on_editBtn_clicked();
    void on_deleteBtn_clicked();
    void on_clearBtn_clicked();
    void on_searchLineEdit_textChanged(const QString &text);

private:
    Ui::MainWindow *ui;

    void setupTable();
    void loadContacts(const QString &filterText = "");
    int selectedContactId() const;
};

#endif // MAINWINDOW_H


