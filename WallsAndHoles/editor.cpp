#include "editor.h"
#include "ui_editor.h"
#include "rectcell.h"
#include <QDebug>
#include <QGraphicsScene>
Editor::Editor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Editor)
{
    ui->setupUi(this);
    rectCell *r = new rectCell(0,0,0,0);
    qDebug() << r->x();
}

Editor::~Editor()
{
    delete ui;
}

void Editor::paintEvent(QPaintEvent *painter)
{
    QGraphicsScene *scene = new QGraphicsScene;
    ui->graphicsView->setScene(scene);
    for(int i= 0; i < ui->graphicsView->height(); i+=30){
        for(int j= 0; j < ui->graphicsView->width(); j+=30){
            scene->addRect(i,j,30,30);
        }
    }
}

void Editor::clear()
{

}

void Editor::addTile()
{

}
