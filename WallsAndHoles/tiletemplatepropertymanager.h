#ifndef TILETEMPLATEPROPERTYMANAGER_H
#define TILETEMPLATEPROPERTYMANAGER_H

#include "abstractpropertymanager.h"
#include "tiletemplate.h"

class TileTemplatePropertyManager : public AbstractPropertyManager
{
    Q_OBJECT

public:
    TileTemplatePropertyManager(TileTemplate *tileTemplate);

    void propertyEdited(QString propertyName, QVariant value) override;

    QVector<QVector<QVariant>> properties() override;

private:
    TileTemplate *mTileTemplate;
};

#endif // TILETEMPLATEPROPERTYMANAGER_H
