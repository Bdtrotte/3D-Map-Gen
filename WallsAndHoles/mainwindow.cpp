

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "meshviewcontainer.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mMeshViewContainer(nullptr)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mMeshViewContainer;
}


void MainWindow::openMeshView() {
    if (mMeshViewContainer == nullptr) {
        mMeshViewContainer = new MeshViewContainer();
        mMeshViewContainer->show();
    }
}

void MainWindow::on_pushButton_clicked() {
    openMeshView();
}
