#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private:
    Ui::MainWindow *ui;

    MeshViewContainer *mMeshViewContainer;
};

#endif // MAINWINDOW_H
