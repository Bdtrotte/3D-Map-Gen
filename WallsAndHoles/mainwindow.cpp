

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    meshView(nullptr)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete meshView;
}


void MainWindow::openMeshView() {
    if (meshView == nullptr) {

        QSharedPointer<Scene> scene = QSharedPointer<Scene>::create();
        /* TESTING
        scene->addObject(QSharedPointer<RenderableObject>::create((QVector<QVector3D>)
        {
           QVector3D(0, 0, 0),
           QVector3D(0, 1, 0),
           QVector3D(1, 1, 0),
           QVector3D(1, 1, 0),
           QVector3D(1, 1, -1),
           QVector3D(0, 1, 0)
        }));
        scene->addObject(QSharedPointer<RenderableObject>::create((QVector<QVector3D>)
        {
           QVector3D(0, 0, 0),
           QVector3D(0, -3, 0),
           QVector3D(-3, -3, 0)
        }));
         TESTING */
        scene->addObject(loadOBJ(":/assets/cube.obj"));

        meshView = new MeshView(scene, this);
        meshView->show();
    }
}

void MainWindow::on_pushButton_clicked() {
    openMeshView();
}
