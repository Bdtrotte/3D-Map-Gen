#include "tiletemplateeditor.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

TileTemplateEditor::TileTemplateEditor(QWidget *parent)
    : QWidget(parent)
    , mNameLable(new QLabel(tr("Name"), this))
    , mName(new QLineEdit(this))
    , mHeightLable(new QLabel(tr("Height"), this))
    , mHeight(new QDoubleSpinBox(this))
    , mThicknessLable(new QLabel(tr("Thickness"), this))
    , mThickness(new QDoubleSpinBox(this))
    , mXPositionLabel(new QLabel(tr("X Position"), this))
    , mXPosition(new QDoubleSpinBox(this))
    , mYPositionLabel(new QLabel(tr("Y Position"), this))
    , mYPosition(new QDoubleSpinBox(this))
    , mTileTemplate(nullptr)
    , mImEditing(false)
{
    // TODO define height range somewhere
    mHeight->setRange(-1000, 1000);
    mThickness->setRange(0.01, 1);
    mXPosition->setRange(0, 1);
    mYPosition->setRange(0, 1);

    QHBoxLayout *hLayout = new QHBoxLayout;
    QVBoxLayout *vLayout = new QVBoxLayout;

    vLayout->addWidget(mNameLable);
    vLayout->addWidget(mHeightLable);
    vLayout->addWidget(mThicknessLable);
    vLayout->addWidget(mXPositionLabel);
    vLayout->addWidget(mYPositionLabel);

    hLayout->addLayout(vLayout);

    vLayout = new QVBoxLayout;
    vLayout->addWidget(mName);
    vLayout->addWidget(mHeight);
    vLayout->addWidget(mThickness);
    vLayout->addWidget(mXPosition);
    vLayout->addWidget(mYPosition);

    hLayout->addLayout(vLayout);

    setLayout(hLayout);

    connect(mName, &QLineEdit::textChanged,
            this, &TileTemplateEditor::nameChanged);
    connect(mHeight, SIGNAL(valueChanged(double)),
            this, SLOT(heightChanged(double)));
    connect(mThickness, SIGNAL(valueChanged(double)),
            this, SLOT(thicknessChanged(double)));
    connect(mXPosition, SIGNAL(valueChanged(double)),
            this, SLOT(xPositionChanged(double)));
    connect(mYPosition, SIGNAL(valueChanged(double)),
            this, SLOT(yPositionChanged(double)));

    setUpEditor();
}

void TileTemplateEditor::tileTemplateChanged(TileTemplate *tileTemplate)
{
    disconnect(mTileTemplate);

    mTileTemplate = tileTemplate;
    if (mTileTemplate != nullptr) {
        connect(mTileTemplate, &TileTemplate::changed,
                this, &TileTemplateEditor::tileTemplatePropertyChanged);
    }

    setUpEditor();
}

void TileTemplateEditor::tileTemplatePropertyChanged()
{
    if (!mImEditing)
        setUpEditor();
}

void TileTemplateEditor::nameChanged(QString value)
{
    if (mTileTemplate == nullptr)
        return;

    mImEditing = true;

    mTileTemplate->setName(value);

    mImEditing = false;
}

void TileTemplateEditor::heightChanged(double value)
{
    if (mTileTemplate == nullptr)
        return;

    mImEditing = true;

    mTileTemplate->setHeight(value);

    mImEditing = false;
}

void TileTemplateEditor::thicknessChanged(double value)
{
    if (mTileTemplate == nullptr)
        return;

    mImEditing = true;

    mTileTemplate->setThickness(value);

    mImEditing = false;
}

void TileTemplateEditor::xPositionChanged(double value)
{
    if (mTileTemplate == nullptr)
        return;

    mImEditing = true;

    QVector2D pos = mTileTemplate->position();
    pos.setX(value);
    mTileTemplate->setPosition(pos);

    mImEditing = false;
}

void TileTemplateEditor::yPositionChanged(double value)
{
    if (mTileTemplate == nullptr)
        return;

    mImEditing = true;

    QVector2D pos = mTileTemplate->position();
    pos.setY(value);
    mTileTemplate->setPosition(pos);

    mImEditing = false;
}

void TileTemplateEditor::setUpEditor()
{
    if (mTileTemplate == nullptr) {
        mName->setText("");
        mHeight->setValue(0);
        mThickness->setValue(1);
        mXPosition->setValue(0.5);
        mYPosition->setValue(0.5);

        mName->setEnabled(false);
        mHeight->setEnabled(false);
        mThickness->setEnabled(false);
        mXPosition->setEnabled(false);
        mYPosition->setEnabled(false);
    } else {
        //otherwise changed slots are called redundantly.
        mName->blockSignals(true);
        mHeight->blockSignals(true);
        mThickness->blockSignals(true);
        mXPosition->blockSignals(true);
        mYPosition->blockSignals(true);

        mName->setText(mTileTemplate->name());
        mHeight->setValue(mTileTemplate->height());
        mThickness->setValue(mTileTemplate->thickness());
        mXPosition->setValue(mTileTemplate->position().x());
        mYPosition->setValue(mTileTemplate->position().y());

        mName->setEnabled(true);
        mHeight->setEnabled(true);
        mThickness->setEnabled(true);
        mXPosition->setEnabled(true);
        mYPosition->setEnabled(true);

        mName->blockSignals(false);
        mHeight->blockSignals(false);
        mThickness->blockSignals(false);
        mXPosition->blockSignals(false);
        mYPosition->blockSignals(false);
    }
}
