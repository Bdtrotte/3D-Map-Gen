#include "colorpickerbutton.h"

#include <QColorDialog>
#include <QPixmap>

ColorPickerButton::ColorPickerButton(QColor color, QWidget *parent)
    : QPushButton(parent)
    , mColor(color)
{
    setColor();
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    connect(this, &QPushButton::clicked,
            this, &ColorPickerButton::clicked);
}

void ColorPickerButton::colorChanged(QColor color)
{
    mColor = color;
    setColor();
}

void ColorPickerButton::clicked()
{
    QColorDialog cd;
    cd.setCurrentColor(mColor);

    if (cd.exec()) {
        colorChanged(cd.currentColor());
        emit colorChanged(mColor);
    }
}

void ColorPickerButton::setColor()
{
    QPixmap color(20, 20);
    color.fill(mColor);
    setIcon(color);
}
