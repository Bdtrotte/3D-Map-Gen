#include "tiletemplateeditor.h"

#include <QGridLayout>
#include <QLabel>

TileTemplateEditor::TileTemplateEditor(QWidget *parent)
    : QWidget(parent)
    , mName(new QLineEdit(this))
    , mHeight(new QDoubleSpinBox(this))
    , mThickness(new QDoubleSpinBox(this))
    , mXPosition(new QDoubleSpinBox(this))
    , mYPosition(new QDoubleSpinBox(this))
    , mTileTemplate(nullptr)
    , mImEditing(false)
{
    // TODO define height range somewhere
    mHeight->setRange(-1000, 1000);
    mThickness->setRange(0.01, 1);
    mXPosition->setRange(0.01, 0.99);
    mYPosition->setRange(0.01, 0.99);

    mColorPickButton = new ColorPickerButton(Qt::black, this);

    QGridLayout *layout = new QGridLayout;

    layout->addWidget(new QLabel(tr("Name"), this), 0, 0);
    layout->addWidget(new QLabel(tr("Color"), this), 1, 0);
    layout->addWidget(new QLabel(tr("Height"), this), 2, 0);
    layout->addWidget(new QLabel(tr("Thickness"), this), 3, 0);
    layout->addWidget(new QLabel(tr("X Position"), this), 4, 0);
    layout->addWidget(new QLabel(tr("Y Position"), this), 5, 0);

    layout->addWidget(mName, 0, 1);
    layout->addWidget(mColorPickButton, 1, 1);
    layout->addWidget(mHeight, 2, 1);
    layout->addWidget(mThickness, 3, 1);
    layout->addWidget(mXPosition, 4, 1);
    layout->addWidget(mYPosition, 5, 1);

    setLayout(layout);

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
    connect(mColorPickButton, &ColorPickerButton::colorPicked,
            this, &TileTemplateEditor::colorChanged);

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

void TileTemplateEditor::colorChanged(QColor color)
{
    Q_ASSERT(mTileTemplate != nullptr);

    mImEditing = true;

    mTileTemplate->setColor(color);

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
        mColorPickButton->colorChanged(Qt::black);

        mName->setEnabled(false);
        mHeight->setEnabled(false);
        mThickness->setEnabled(false);
        mXPosition->setEnabled(false);
        mYPosition->setEnabled(false);
        mColorPickButton->setEnabled(false);
    } else {
        //otherwise changed slots are called redundantly.
        mName->blockSignals(true);
        mHeight->blockSignals(true);
        mThickness->blockSignals(true);
        mXPosition->blockSignals(true);
        mYPosition->blockSignals(true);
        mColorPickButton->blockSignals(true);

        mName->setText(mTileTemplate->name());
        mHeight->setValue(mTileTemplate->height());
        mThickness->setValue(mTileTemplate->thickness());
        mXPosition->setValue(mTileTemplate->position().x());
        mYPosition->setValue(mTileTemplate->position().y());
        mColorPickButton->colorChanged(mTileTemplate->color());

        mName->setEnabled(true);
        mHeight->setEnabled(true);
        mThickness->setEnabled(true);
        mXPosition->setEnabled(true);
        mYPosition->setEnabled(true);
        mColorPickButton->setEnabled(true);

        mName->blockSignals(false);
        mHeight->blockSignals(false);
        mThickness->blockSignals(false);
        mXPosition->blockSignals(false);
        mYPosition->blockSignals(false);
        mColorPickButton->blockSignals(false);
    }
}
