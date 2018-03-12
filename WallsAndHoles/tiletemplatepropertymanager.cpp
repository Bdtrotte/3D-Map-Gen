#include "tiletemplatepropertymanager.h"

#include "propertybrowser.h"
#include "templatematerialselector.h"

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
        if (value.toFloat() != mTileTemplate->setThickness(value.toFloat()))
            emit propertyChanged(propertyName, mTileTemplate->thickness());
    } else if (propertyName == "X Position") {
        QVector2D pos = mTileTemplate->position();
        pos.setX(value.toFloat());
        if (pos != mTileTemplate->setPosition(pos))
            emit propertyChanged(propertyName, mTileTemplate->position().x());
    } else if (propertyName == "Y Position") {
        QVector2D pos = mTileTemplate->position();
        pos.setY(value.toFloat());
        if (pos != mTileTemplate->setPosition(pos))
            emit propertyChanged(propertyName, mTileTemplate->position().y());
    }
}

QVector<QVector<QVariant>> TileTemplatePropertyManager::properties()
{
    TemplateMaterialSelector *materialSelector = new TemplateMaterialSelector(mTileTemplate);

    return {
        {"Name",       mTileTemplate->name(),         true              },
        {"Color",      mTileTemplate->color(),        true              },
        {"Height",     mTileTemplate->height(),       true,  -1000, 1000},
        {"Thickness",  mTileTemplate->thickness(),    true,  -1000, 1000},
        //{"X Position", mTileTemplate->position().x(), true,  -1000, 1000},
        //{"Y Position", mTileTemplate->position().y(), true,  -1000, 1000},
        {"Material", QVariant::fromValue<TemplateMaterialSelector *>(materialSelector),
                     true,
                     PropertyBrowser::BlockWidget}
    };
}
