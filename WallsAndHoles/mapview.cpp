#include "mapview.h"
#include "ui_mapview.h"
#include "rectcell.h"
#include <QDebug>
#include <QGraphicsScene>
MapView::MapView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MapView)
{
    ui->setupUi(this);
    rectCell *r = new rectCell(0,0,0,0);
    qDebug() << r->x();
}

MapView::~MapView()
{
    delete ui;
}

void MapView::paintEvent(QPaintEvent *painter)
{
    QGraphicsScene *scene = new QGraphicsScene;
    ui->graphicsView->setScene(scene);
    for(int i= 0; i < ui->graphicsView->height(); i+=30){
        for(int j= 0; j < ui->graphicsView->width(); j+=30){
            scene->addRect(i,j,30,30);
        }
    }
}

void MapView::clear()
{

}

void MapView::addTile()
{

}
