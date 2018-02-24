#ifndef MATERIALPROPERTYMANAGER_H
#define MATERIALPROPERTYMANAGER_H

#include "abstractpropertymanager.h"
#include "tilematerial.h"
#include "imageandsource.h"

class MaterialPropertyManager : public AbstractPropertyManager
{
    Q_OBJECT

public:
    MaterialPropertyManager(TileMaterial *material);

    void propertyEdited(QString propertyName, QVariant value) override;

    QVector<QVector<QVariant>> properties() override;

private slots:
    void textureChanged(SharedImageAndSource imageAndSource);

private:
    TileMaterial *mMaterial;
};

#endif // MATERIALPROPERTYMANAGER_H
