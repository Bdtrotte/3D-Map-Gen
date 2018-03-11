#include "templatematerialselector.h"

#include <QCheckBox>
#include <QHBoxLayout>
#include <QVBoxLayout>

TemplateMaterialSelector::TemplateMaterialSelector(TileTemplate *tileTemplate)
    : QWidget()
    , mTileTemplate(tileTemplate)
{
    TileMaterialSelectionBar *topMaterialSelectionBar = new TileMaterialSelectionBar(mTileTemplate->topMaterial(), this);

    mSideMaterialSelectionBar = new TileMaterialSelectionBar(mTileTemplate->sideMaterial(), this);

    QCheckBox *checkBox = new QCheckBox(this);

    mTopMaterialLabel = new QLabel(this);
    QLabel *checkBoxLabel = new QLabel("Has Side material", this);
    mSideMaterialLabel = new QLabel("Side Material", this);

    QVBoxLayout *layout = new QVBoxLayout;
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(checkBoxLabel);
    hLayout->addWidget(checkBox);
    layout->addLayout(hLayout);
    hLayout = new QHBoxLayout;
    hLayout->addWidget(mTopMaterialLabel);
    hLayout->addWidget(topMaterialSelectionBar);
    layout->addLayout(hLayout);
    hLayout = new QHBoxLayout;
    hLayout->addWidget(mSideMaterialLabel);
    hLayout->addWidget(mSideMaterialSelectionBar);
    layout->addLayout(hLayout);

    setLayout(layout);

    if (mTileTemplate->hasSideMaterial()) {
        mTopMaterialLabel->setText("Top Material");
        checkBox->setChecked(true);
    } else {
        mTopMaterialLabel->setText("Material");
        checkBox->setChecked(false);
        mSideMaterialSelectionBar->setVisible(false);
        mSideMaterialLabel->setVisible(false);
    }

    connect(topMaterialSelectionBar, &TileMaterialSelectionBar::materialChanged,
            this, &TemplateMaterialSelector::topMaterialChanged);
    connect(mTileTemplate, &TileTemplate::materialChanged,
            topMaterialSelectionBar, [this, topMaterialSelectionBar]() {
        if (topMaterialSelectionBar->material() != mTileTemplate->topMaterial())
            topMaterialSelectionBar->setMaterial(mTileTemplate->topMaterial());
    });

    connect(mSideMaterialSelectionBar, &TileMaterialSelectionBar::materialChanged,
            this, &TemplateMaterialSelector::sideMaterialChanged);
    connect(mTileTemplate, &TileTemplate::materialChanged,
            mSideMaterialSelectionBar, [this]() {
        if (mSideMaterialSelectionBar->material() != mTileTemplate->sideMaterial())
            mSideMaterialSelectionBar->setMaterial(mTileTemplate->sideMaterial());
    });

    connect(checkBox, &QCheckBox::toggled,
            this, &TemplateMaterialSelector::checkBoxToggled);
}

void TemplateMaterialSelector::topMaterialChanged(TileMaterial *material)
{
    mTileTemplate->setTopMaterial(material);
}

void TemplateMaterialSelector::sideMaterialChanged(TileMaterial *material)
{
    mTileTemplate->setSideMaterial(material);
}

void TemplateMaterialSelector::checkBoxToggled(bool state)
{
    if (state == mTileTemplate->hasSideMaterial()) return;

    mTileTemplate->setHasSideMaterial(state);

    if (state) {
        mTopMaterialLabel->setText("Top Material");
        mSideMaterialLabel->setVisible(true);
        mSideMaterialSelectionBar->setVisible(true);
    } else {
        mTopMaterialLabel->setText("Material");
        mSideMaterialLabel->setVisible(false);
        mSideMaterialSelectionBar->setVisible(false);
    }
}
