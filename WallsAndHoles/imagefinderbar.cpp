#include "imagefinderbar.h"

#include <QFileDialog>
#include <QPushButton>
#include <QHBoxLayout>

// TODO: Add image preview

ImageFinderBar::ImageFinderBar(QString source, QWidget *parent)
    : QWidget(parent)
    , mLine(new QLineEdit)
{
    mLine->setReadOnly(true);
    mLine->setText(source);

    QPushButton *button = new QPushButton("...", this);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(mLine);
    layout->addWidget(button);

    setLayout(layout);

    connect(button, &QPushButton::clicked,
            this, &ImageFinderBar::findImage);
}

void ImageFinderBar::findImage()
{
    QString path = QFileDialog::getOpenFileName(this,
                                                tr("Open Image"),
                                                "/home/",
                                                tr("Image Files (*.png, *.jpg)")); //TODO Add more supported image formats

    if (path.isEmpty()) return;

    SharedImageAndSource imageAndSource = ImageAndSource::getSharedImageAndSource(path);
    if (imageAndSource->image()->isNull())
        return;

    mLine->setText(imageAndSource->source());

    emit imageChanged(imageAndSource);
}
