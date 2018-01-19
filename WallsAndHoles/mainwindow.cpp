

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
        /* TESTING */
        QSharedPointer<Scene> scene = QSharedPointer<Scene>::create();
        scene->addObject(QSharedPointer<RenderableObject>::create((QVector<QVector3D>)
        {
           QVector3D(0, 0, -1),
           QVector3D(0, 1, -1),
           QVector3D(1, 1, -1)
        }));
        /* TESTING */

        meshView = new MeshView(scene, this);
        meshView->show();
    }
}

void MainWindow::on_pushButton_clicked() {
    openMeshView();
}
