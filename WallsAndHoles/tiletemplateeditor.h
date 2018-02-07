#ifndef TILETEMPLATEEDITOR_H
#define TILETEMPLATEEDITOR_H

#include "tiletemplate.h"

#include <QWidget>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QLabel>

class TileTemplateEditor : public QWidget
{
    Q_OBJECT

public:
    explicit TileTemplateEditor(QWidget *parent = nullptr);

public slots:
    void tileTemplateChanged(SharedTileTemplate tileTemplate);

private slots:
    void tileTemplatePropertyChanged();

    void nameChanged(QString value);
    void heightChanged(double value);
    void thicknessChanged(double value);
    void xPositionChanged(double value);
    void yPositionChanged(double value);
    //void colorChange();

private:
    /**
     * @brief setUpEditor
     * Fills the editor with all the data from mTileTemplate
     */
    void setUpEditor();

    QLabel *mNameLable;
    QLineEdit *mName;
    QLabel *mHeightLable;
    QDoubleSpinBox *mHeight;
    QLabel *mThicknessLable;
    QDoubleSpinBox *mThickness;
    QLabel *mXPositionLabel;
    QDoubleSpinBox *mXPosition;
    QLabel *mYPositionLabel;
    QDoubleSpinBox *mYPosition;

    SharedTileTemplate mTileTemplate;

    //set to true when changes are being made within this editor
    //to avoid loops caused by signals
    bool mImEditing;
};

#endif // TILETEMPLATEEDITOR_H
