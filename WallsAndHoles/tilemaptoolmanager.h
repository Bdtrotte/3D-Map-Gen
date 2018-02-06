#ifndef TILEMAPTOOLMANAGER_H
#define TILEMAPTOOLMANAGER_H

#include "abstracttilemaptool.h"
#include "tilemap.h"
#include "toolmanager.h"

class TileMapToolManager : public ToolManager
{
    Q_OBJECT

public:
    TileMapToolManager(QObject *parent = nullptr);

    void setTileMap(TileMap *tileMap);

    QAction *registerMapTool(QSharedPointer<AbstractTileMapTool> tool, QString name) { return ToolManager::registerTool(tool, name); }

public slots:
    void cellActivated(int x, int y);
    void cellClicked(int x, int y);
    void cellReleased(int x, int y);

private:
    using ToolManager::registerTool;
    using ToolManager::mousePressEvent;
    using ToolManager::mouseMoveEvent;
    using ToolManager::mouseReleaseEvent;
};

#endif // TILEMAPTOOLMANAGER_H
