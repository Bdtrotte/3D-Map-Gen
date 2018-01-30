

#include "mainwindow.h"
#include "meshviewcontainer.h"
#include "mapview.h"
#include "ui_mainwindow.h"

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

void MainWindow::on_actionopen_triggered()
{
    MapView *mapView = findChild<MapView *>();

    if (mapView == nullptr) {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","mapView doesn't exist!");
        messageBox.setFixedSize(500,200);
        return;
    }
    /*
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Map"), "/home", tr("Open Files (*.xml)"));
    */
    QString fileName = "test.xml";
    if(!fileName.isEmpty())
        mapView->openMap(fileName);
}

void MainWindow::on_actionsave_triggered()
{
    MapView *mapView = findChild<MapView *>();

    if (mapView == nullptr) {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","mapView doesn't exist!");
        messageBox.setFixedSize(500,200);
        return;
    }
    /*
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save Map"), "/home", tr("Save Files (*.xml)"));*/
    QString fileName = "test.xml";
    if(!fileName.isEmpty())
        mapView->saveMap(fileName);
}

void MainWindow::on_actionexport_triggered() {
    MeshViewContainer *meshViewContainer = findChild<MeshViewContainer *>();

    if (meshViewContainer == nullptr) {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","MeshView doesn't exist!");
        messageBox.setFixedSize(500,200);
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Export OBJ"), "/home", tr("Export Files (*.obj)"));

    if(!fileName.isEmpty())
        meshViewContainer->saveMesh(fileName);
}

void MainWindow::on_actionimport_triggered() {
    MeshViewContainer *meshViewContainer = findChild<MeshViewContainer *>();

    if (meshViewContainer == nullptr) {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","MeshView doesn't exist!");
        messageBox.setFixedSize(500,200);
        return;
    }

    QString fileName = QFileDialog::getOpenFileName(this,
        tr("import OBJ"), "/home", tr("import Files (*.obj)"));

    if(!fileName.isEmpty())
        meshViewContainer->loadMesh(fileName);
}
