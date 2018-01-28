#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>

#include "meshviewcontainer.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void openMeshView();

private slots:
    void on_pushButton_clicked();

    void on_actionopen_triggered();

    void on_actionsave_triggered();

    void on_actionexport_triggered();

    void on_actionimport_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
