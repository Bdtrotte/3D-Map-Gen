#ifndef COLORPICKERBUTTON_H
#define COLORPICKERBUTTON_H

#include <QPushButton>

class ColorPickerButton : public QPushButton
{
    Q_OBJECT

public:
    ColorPickerButton(QColor color = Qt::black, QWidget *parent = nullptr);

    QColor currentColor() const { return mColor; }

signals:
    void colorPicked(QColor color);

public slots:
    /**
     * @brief colorChanged
     * When the color this button represents changes by means other than the button.
     * Does not emit colorPicked;
     * @param color
     */
    void colorChanged(QColor color);

private slots:
    void clicked();

private:
    //sets the buttons icon to mColor
    void setColor();

    QColor mColor;
};

#endif // COLORPICKERBUTTON_H
