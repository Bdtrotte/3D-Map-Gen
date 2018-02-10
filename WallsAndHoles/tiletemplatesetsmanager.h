#ifndef TILETEMPLATESETSMANAGER_H
#define TILETEMPLATESETSMANAGER_H

#include "tilemap.h"
#include "tiletemplatesetsview.h"
#include "savabletiletemplateset.h"

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
    explicit TileTemplateSetsManager(TileMap *tileMap = nullptr, QObject *parent = nullptr);

    /**
     * @brief newTileTemplateSet
     * Opens dialogs to create a new tileTemplateSet
     */
    void newTileTemplateSet();

    /**
     * @brief addTileTemplateSet
     * Adds the given tileTemplateSet to the application
     * @param tileTemplateSet
     */
    void addTileTemplateSet(SavableTileTemplateSet *tileTemplateSet);

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
    bool removeTileTemplateSet(SavableTileTemplateSet *TileTemplateSet);

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
     * Opens dialogs to locate the templateSet to load, then loads it
     * @return
     */
    SavableTileTemplateSet *loadTileTemplateSet();

    /**
     * @brief loadTileTemplateSet
     * Loads a tileTemplateSet at the given path.
     * If the file can't be loaded for any reason,
     * the user will be given dialogs to relocate the file.
     * @param path
     */
    SavableTileTemplateSet *loadTileTemplateSet(QString path);

    SavableTileTemplateSet *tileTemplateSetAt(int i) { return mTileTemplateSets[i]; }
    const QList<SavableTileTemplateSet *> tileTemplateSets() { return mTileTemplateSets; }

signals:
    /**
     * @brief tileTemplateSetAdded
     * Emitted whenever a new TileTemplateSet is added to the manager:
     * Either by loading, new dialog, or somewhere in code.
     * @param tileTemplateSet
     */
    void tileTemplateSetAdded(TileTemplateSet *tileTemplateSet);

    /**
     * @brief tileTemplateSetAboutToBeRemoved
     * emitted right before the given tileTemplateSet is removed and deleted.
     * @param tileTemplateSet
     */
    void tileTemplateSetAboutToBeRemoved(TileTemplateSet *tileTemplateSet);

private:
    TileMap *mTileMap;

    QList<SavableTileTemplateSet *> mTileTemplateSets;
};

#endif // TILETEMPLATESETSMANAGER_H
