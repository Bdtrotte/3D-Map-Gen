#include "mapview.h"
#include "ui_mapview.h"
#include "rectcell.h"
#include <QDebug>
#include <QGraphicsScene>
#include <QResizeEvent>
#include <QMatrix>


MapView::MapView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MapView)
{
    ui->setupUi(this);


    QGraphicsScene *scene = new QGraphicsScene;
    scene->setBackgroundBrush(Qt::gray);
    ui->graphicsView->setScene(scene);
    for(qreal i = 0; i < ui->graphicsView->height()-50; i+=30){
        for(qreal j= 0; j < ui->graphicsView->width()-50; j+=30){
            rectCell *temp = new rectCell(j,i,30,30);
            scene->addItem(temp);
        }
    }
}

MapView::~MapView()
{
    delete ui;
}

void MapView::resizeEvent(QResizeEvent *q)
{

}


void MapView::on_horizontalSlider_sliderMoved(int position)
{
    float scale = position*position/100;
    QMatrix mat;
    mat.scale(scale,scale);
    ui->graphicsView->setMatrix(mat);

}
