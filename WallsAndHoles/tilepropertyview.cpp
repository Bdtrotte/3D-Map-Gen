#include "tilepropertyview.h"

TilePropertyView::TilePropertyView(QWidget *parent)
    : QWidget(parent)
    , mRelativeHeightLabel(new QLabel(tr("RelativeHeight"), this))
    , mRelativeHeight(new QDoubleSpinBox(this))
    , mRelativeThicknessLabel(new QLabel(tr("RelativeThickness"), this))
    , mRelativeThickness(new QDoubleSpinBox(this))
    , mRelativeXPositionLabel(new QLabel(tr("RelativeXPosition"), this))
    , mRelativeXPosition(new QDoubleSpinBox(this))
    , mRelativeYPositionLabel(new QLabel(tr("RelativeYPosition"), this))
    , mRelativeYPosition(new QDoubleSpinBox(this))
    , mTile(nullptr)
{
    mRelativeHeight->setRange(-10, 10);
    mRelativeHeight->setSingleStep(0.1);
    mRelativeThickness->setRange(-1, 1);
    mRelativeThickness->setSingleStep(0.1);
    mRelativeXPosition->setRange(-0.49, 0.49);
    mRelativeXPosition->setSingleStep(0.1);
    mRelativeYPosition->setRange(-0.49, 0.49);
    mRelativeYPosition->setSingleStep(0.1);

    QHBoxLayout *hLayout = new QHBoxLayout;
    QVBoxLayout *vLayout = new QVBoxLayout;

    vLayout->addWidget(mRelativeHeightLabel);
    vLayout->addWidget(mRelativeThicknessLabel);
    vLayout->addWidget(mRelativeXPositionLabel);
    vLayout->addWidget(mRelativeYPositionLabel);

    hLayout->addLayout(vLayout);

    vLayout = new QVBoxLayout;
    vLayout->addWidget(mRelativeHeight);
    vLayout->addWidget(mRelativeThickness);
    vLayout->addWidget(mRelativeXPosition);
    vLayout->addWidget(mRelativeYPosition);

    hLayout->addLayout(vLayout);

    setLayout(hLayout);

    setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum));

    connect(mRelativeHeight, SIGNAL(valueChanged(double)),
            this, SLOT(relativeHeightChanged(double)));
    connect(mRelativeThickness, SIGNAL(valueChanged(double)),
            this, SLOT(relativeThicknessChanged(double)));
    connect(mRelativeXPosition, SIGNAL(valueChanged(double)),
            this, SLOT(relativeXPositionChanged(double)));
    connect(mRelativeYPosition, SIGNAL(valueChanged(double)),
            this, SLOT(relativeYPositionChanged(double)));
    clear();
}

void TilePropertyView::setTile(Tile *tile)
{
    mTile = tile;
    if(mTile!=nullptr){
        //block signals, otherwise changed slots are called redundantly.
        mRelativeHeight->blockSignals(true);
        mRelativeThickness->blockSignals(true);
        mRelativeXPosition->blockSignals(true);
        mRelativeYPosition->blockSignals(true);

        mRelativeHeight->setValue(mTile->relativeHeight());
        mRelativeThickness->setValue(mTile->relativeThickness());
        mRelativeXPosition->setValue(mTile->relativePosition().x());
        mRelativeYPosition->setValue(mTile->relativePosition().y());

        mRelativeHeight->setEnabled(true);
        mRelativeThickness->setEnabled(true);
        mRelativeXPosition->setEnabled(true);
        mRelativeYPosition->setEnabled(true);

        mRelativeHeight->blockSignals(false);
        mRelativeThickness->blockSignals(false);
        mRelativeXPosition->blockSignals(false);
        mRelativeYPosition->blockSignals(false);
    } else {
        clear();
    }
}

void TilePropertyView::clear(){
    mTile = nullptr;
    mRelativeHeight->setValue(0);
    mRelativeThickness->setValue(0);
    mRelativeXPosition->setValue(0);
    mRelativeYPosition->setValue(0);

    mRelativeHeight->setEnabled(false);
    mRelativeThickness->setEnabled(false);
    mRelativeXPosition->setEnabled(false);
    mRelativeYPosition->setEnabled(false);
}

void TilePropertyView::relativeHeightChanged(double value)
{
    if (mTile!=nullptr){      
        mTile->setRelativeHeight(value);
    }
}

void TilePropertyView::relativeThicknessChanged(double value)
{
    if (mTile!=nullptr)
        mTile->setRelativeThickness(value);
}
void TilePropertyView::relativeXPositionChanged(double value)
{
    if (mTile!=nullptr){
        QVector2D pos = mTile->relativePosition();
        pos.setX(value);
        qDebug() << "now set tile relative position x to:" << value;
        mTile->setRelativePosition(pos);
        qDebug() << "result o:" << mTile->relativePosition().x();
    }
}
void TilePropertyView::relativeYPositionChanged(double value){
    if (mTile!=nullptr){
        QVector2D pos = mTile->relativePosition();
        pos.setY(value);
        mTile->setRelativePosition(pos);
    }
}


