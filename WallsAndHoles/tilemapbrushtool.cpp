#include "tilemapbrushtool.h"
#include "tiletemplatechangecommand.h"

#include <QSpinBox>
#include <QLabel>
#include <QHBoxLayout>

TileMapBrushTool::TileMapBrushTool(TileMapPreviewGraphicsItem *previewItem, QUndoStack *undoStack, QObject *parent)
    : AbstractTileMapTool(previewItem, parent)
    , mRadius(1)
    , mRadiusSpinner(new QWidgetAction(this))
    , mUndoStack(undoStack)
{
    QWidget *w = new QWidget();

    QLabel *label = new QLabel("Radius: ", w);
    QSpinBox *spinBox = new QSpinBox(w);
    spinBox->setRange(1, 99);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(label);
    layout->addWidget(spinBox);
    w->setLayout(layout);

    mRadiusSpinner->setDefaultWidget(w);

    connect(spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, [this](int i) { mRadius = i; });
}

void TileMapBrushTool::cellActivated(int x, int y, QMouseEvent *)
{
    TileMap *tileMap = getTileMap();

    int r = mRadius - 1;
    int d = r * 2 + 1;
    QRegion region(x - r, y - r, d, d, QRegion::Ellipse);
    region &= QRect(QPoint(0, 0), tileMap->mapSize());

    mUndoStack->push(TileTemplateChangeCommand::performCommand(
                         tileMap,
                         region,
                         getTileTemplate(),
                         "'brush tool'"));
}

void TileMapBrushTool::cellHovered(int x, int y, QMouseEvent *)
{
    int r = mRadius - 1;
    int d = r * 2 + 1;
    QRegion region(x - r, y - r, d, d, QRegion::Ellipse);
    region &= QRect(QPoint(0, 0), getTileMap()->mapSize());

    if (TileTemplate *t = getTileTemplate()) {
        mPreviewItem->setRegion(region);
        mPreviewItem->setColor(t->color());
    } else {
        mPreviewItem->setRegion(QRegion());
    }
}

void TileMapBrushTool::mouseExitedMap(QMouseEvent *)
{
    mPreviewItem->setRegion(QRegion());
}
