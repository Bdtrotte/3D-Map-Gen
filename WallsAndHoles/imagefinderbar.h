#ifndef IMAGEFINDERBAR_H
#define IMAGEFINDERBAR_H

#include "imageandsource.h"

#include <QWidget>
#include <QLineEdit>

class ImageFinderBar : public QWidget
{
    Q_OBJECT

public:
    explicit ImageFinderBar(QString source, QWidget *parent = nullptr);

signals:
    void imageChanged(SharedImageAndSource image);

private slots:
    void findImage();

private:
    QLineEdit *mLine;
};

#endif // IMAGEFINDERBAR_H
