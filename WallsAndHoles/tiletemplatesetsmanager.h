#ifndef TILETEMPLATESETSMANAGER_H
#define TILETEMPLATESETSMANAGER_H

#include "tilemap.h"
#include "tiletemplateset.h"

#include <QObject>

/**
 * @brief The TileTemplateSetsManager class
 * Handles the active tileTemplateSets of the application.
 * Can create dialogs to interact with the user.
 * May modify the attached TileMap when removing templateSets.
 */
class TileTemplateSetsManager : public QObject
{
    Q_OBJECT

public:
    explicit TileTemplateSetsManager(QObject *parent = nullptr);

    /**
     * @brief addTileTemplateSet
     * Adds the given tileTemplateSet to the application
     * @param tileTemplateSet
     */
    void addTileTemplateSet(SharedTileTemplateSet tileTemplateSet);

    /**
     * @brief removeTileTemplateSet
     * Attempts to remove the given templateSet from the application.
     * If any of the templates of the set are in use, the user will be
     * presented with a dialog, if they confirm the removal, then any
     * tiles using templates from the set will be erased.
     * If the set is not saved, the user will be prompted to save the set
     * before it is removed.
     * @param TileTemplateSet
     * @return
     */
    bool removeTileTemplateSet(SharedTileTemplateSet TileTemplateSet);

    /**
     * @brief removeTileTemplateSet
     * Attempt to remove a templateSet at the given index.
     * @param index
     * @return
     */
    bool removeTileTemplateSet(int index);

    /**
     * @brief saveAllTileTemplateSets
     * Saves all unsaved tileTemplateSets
     */
    void saveAllTileTemplateSets();

    /**
     * @brief loadTileTemplateSet
     * Loads a tileTemplateSet at the given path.
     * If the file can't be loaded for any reason,
     * the user will be given dialogs to relocate the file.
     * @param path
     */
    SharedTileTemplateSet loadTileTemplateSet(QString path);

    SharedTileTemplateSet tileTemplateSetAt(int i) { return mTileTemplateSets[i]; }
    const QList<SharedTileTemplateSet> tileTemplateSets() { return mTileTemplateSets; }

private:
    QList<SharedTileTemplateSet> mTileTemplateSets;

    TileMap *mTileMap;
};

#endif // TILETEMPLATESETSMANAGER_H
