#include "tiletemplatepropertymanager.h"

#include "tilematerialselectionbar.h"
#include "propertybrowser.h"

TileTemplatePropertyManager::TileTemplatePropertyManager(TileTemplate *tileTemplate)
    : AbstractPropertyManager()
    , mTileTemplate(tileTemplate) {}

void TileTemplatePropertyManager::propertyEdited(QString propertyName, QVariant value)
{
    if (propertyName == "Name") {
        mTileTemplate->setName(value.toString());
    } else if (propertyName == "Color") {
        mTileTemplate->setColor(value.value<QColor>());
    } else if (propertyName == "Height") {
        mTileTemplate->setHeight(value.toFloat());
    } else if (propertyName == "Thickness") {
        mTileTemplate->setThickness(value.toFloat());
    } else if (propertyName == "X Position") {
        QVector2D pos = mTileTemplate->position();
        pos.setX(value.toFloat());
        mTileTemplate->setPosition(pos);
    } else if (propertyName == "Y Position") {
        QVector2D pos = mTileTemplate->position();
        pos.setY(value.toFloat());
        mTileTemplate->setPosition(pos);
    }
}

QVector<QVector<QVariant>> TileTemplatePropertyManager::properties()
{
    TileMaterialSelectionBar *materialSelectionBar = new TileMaterialSelectionBar(mTileTemplate->material());

    return {
        {"Name",       mTileTemplate->name(),         true              },
        {"Color",      mTileTemplate->color(),        true              },
        {"Height",     mTileTemplate->height(),       true,  -1000, 1000},
        {"Thickness",  mTileTemplate->thickness(),    false,   0.1,    1},
        {"X Position", mTileTemplate->position().x(), false, -0.49, 0.49},
        {"Y Position", mTileTemplate->position().y(), false, -0.49, 0.49},
        {"Material", QVariant::fromValue<TileMaterialSelectionBar *>(materialSelectionBar),
                     true,
                     PropertyBrowser::LineWidget}
    };
}
