#include "savabletiletemplateset.h"

#include "xmltool.h"

SavableTileTemplateSet::SavableTileTemplateSet(QString savePath,
                                               QString name,
                                               bool loadedFromFile,
                                               QObject *parent)
    : TileTemplateSet(name, parent)
    , mSavePath(savePath)
    , mSaved(loadedFromFile) {}

void SavableTileTemplateSet::save()
{
    // TODO be louder when saving fails
    if (XMLTool::saveTileTemplateSet(this) != 0)
        return;

    mSaved = true;
    emit saveStateChanged(mSaved);
}

void SavableTileTemplateSet::changed()
{
    if (mSaved) {
        emit saveStateChanged(false);
    }

    mSaved = false;
}
