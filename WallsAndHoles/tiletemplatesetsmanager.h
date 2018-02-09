#ifndef TILETEMPLATESETSMANAGER_H
#define TILETEMPLATESETSMANAGER_H

#include "tiletemplateset.h"

#include <QObject>

/**
 * @brief The TileTemplateSetsManager class
 * Handles the active tileTemplateSets of the application.
 * Can create dialogs to interact with the user
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
     * @brief saveAllTileTemplateSets
     * Saves all unsaved tileTemplateSets
     */
    void saveAllTileTemplateSets();

    SharedTileTemplateSet tileTemplateSetAt(int i) { return mTileTemplateSets[i]; }
    const QList<SharedTileTemplateSet> tileTemplateSets() { return mTileTemplateSets; }

private:
    QList<SharedTileTemplateSet> mTileTemplateSets;
};

#endif // TILETEMPLATESETSMANAGER_H
