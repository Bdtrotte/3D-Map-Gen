#ifndef TILETEMPLATEEDITOR_H
#define TILETEMPLATEEDITOR_H

#include "tiletemplate.h"

#include <QWidget>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QPushButton>

class TileTemplateEditor : public QWidget
{
    Q_OBJECT

public:
    explicit TileTemplateEditor(QWidget *parent = nullptr);

public slots:
    void tileTemplateChanged(TileTemplate *tileTemplate);

private slots:
    void tileTemplatePropertyChanged();

    void nameChanged(QString value);
    void heightChanged(double value);
    void thicknessChanged(double value);
    void xPositionChanged(double value);
    void yPositionChanged(double value);
    void colorChangeClicked();

private:
    /**
     * @brief setUpEditor
     * Fills the editor with all the data from mTileTemplate
     */
    void setUpEditor();

    QLineEdit *mName;
    QDoubleSpinBox *mHeight;
    QDoubleSpinBox *mThickness;
    QDoubleSpinBox *mXPosition;
    QDoubleSpinBox *mYPosition;
    QPushButton *mColorPickButton;

    TileTemplate *mTileTemplate;

    //set to true when changes are being made within this editor
    //to avoid loops caused by signals
    bool mImEditing;
};

#endif // TILETEMPLATEEDITOR_H
