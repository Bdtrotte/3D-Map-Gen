#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mapview.h"
#include "ui_mapview.h"
#include "tiletemplate.h"
#include "tilemap.h"
#include "tile.h"
#include "QDebug"

MainWindow:: MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)

{

    ui->setupUi(this);
    MapView *edit = new MapView;
    edit->show();
    hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

