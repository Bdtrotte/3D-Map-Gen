#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mapview.h"
#include "ui_mapview.h"
#include "QDebug"

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

void MainWindow::on_btn0_clicked()
{
    MapView *edit = new MapView;
    edit->show();
    hide();
}
