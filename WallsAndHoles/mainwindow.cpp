#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "editor.h"
#include "ui_editor.h"
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
    Editor *edit = new Editor;
    edit->show();
    hide();
}
