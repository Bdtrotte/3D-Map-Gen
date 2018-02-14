#ifndef SAVABLETILETEMPLATESET_H
#define SAVABLETILETEMPLATESET_H

#include "tiletemplateset.h"

/**
 * @brief The SavableTileTemplateSet class
 * A TileTemplateSet which is tightly bound to a file.
 * Needs to be used with any custom TileTemplateSet (anything
 * other that default map tileTemplates).
 */
class SavableTileTemplateSet : public TileTemplateSet
{
    Q_OBJECT

public:
    explicit SavableTileTemplateSet(QString savePath,
                                    QString name = "New Tile Template Set",
                                    bool loadedFromFile = false,
                                    QObject *parent = nullptr);

    const QString savePath() const { return mSavePath; }
    void setSavePath(QString path){ mSavePath = path; }

    void save();
    bool isSaved() const { return mSaved; }

signals:
    void saveStateChanged(bool state);

protected:
    void changed() override;

private:
    //default save path of this tileTempalteSet object
    QString mSavePath;

    //whether or not the current state of this is saved. (made false when this is changed)
    bool mSaved;
};

#endif // SAVABLETILETEMPLATESET_H
