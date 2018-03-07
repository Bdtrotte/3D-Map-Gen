#ifndef TEMPLATEMATERIALSELECTOR_H
#define TEMPLATEMATERIALSELECTOR_H

#include "tilematerialselectionbar.h"
#include "tiletemplate.h"

#include <QWidget>
#include <QLabel>

class TemplateMaterialSelector : public QWidget
{
    Q_OBJECT

public:
    TemplateMaterialSelector(TileTemplate *tileTemplate);

private slots:
    void topMaterialChanged(TileMaterial *material);

    void sideMaterialChanged(TileMaterial *material);

    void checkBoxToggled(bool state);

private:
    TileTemplate *mTileTemplate;

    QLabel *mTopMaterialLabel;
    QLabel *mSideMaterialLabel;

    TileMaterialSelectionBar *mSideMaterialSelectionBar;
};

#endif // TEMPLATEMATERIALSELECTOR_H
