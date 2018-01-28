

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mapview.h"
#include "QDebug"
#include "meshviewcontainer.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openMeshView() {
    MeshViewContainer* mMeshViewContainer = findChild<MeshViewContainer *>();
    if (mMeshViewContainer == nullptr) {
        mMeshViewContainer = new MeshViewContainer();
        mMeshViewContainer->show();
    }
}

void MainWindow::on_pushButton_clicked() {
    openMeshView();
}


void MainWindow::on_actionopen_triggered()
{
}

void MainWindow::on_actionsave_triggered()
{
}

void MainWindow::on_actionexport_triggered()
{   MeshViewContainer* mMeshViewContainer = findChild<MeshViewContainer *>();
    if (mMeshViewContainer == nullptr) {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","MeshView doesn't exist!");
        messageBox.setFixedSize(500,200);
        return;
    }
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Export OBJ"), "/home", tr("Export Files (*.obj)"));
    if(!fileName.isEmpty())
        mMeshViewContainer->saveMesh(fileName);
}

void MainWindow::on_actionimport_triggered(){
    MeshViewContainer* mMeshViewContainer = findChild<MeshViewContainer *>();
    if (mMeshViewContainer == nullptr) {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","MeshView doesn't exist!");
        messageBox.setFixedSize(500,200);
        return;
    }
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("import OBJ"), "/home", tr("import Files (*.obj)"));
    if(!fileName.isEmpty())
        mMeshViewContainer->loadMesh(fileName);
}
