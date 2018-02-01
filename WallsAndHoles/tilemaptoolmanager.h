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

    QAction *registerTool(QSharedPointer<AbstractTileMapTool> tool, QString name) { return ToolManager::registerTool(tool, name); }

    void setTileMap(TileMap *tileMap);

public slots:
    void cellActivated(int x, int y);
    void cellClicked(int x, int y);
    void cellReleased(int x, int y);

private:
    using ToolManager::registerTool;
    using ToolManager::mousePressEvent;
    using ToolManager::mouseReleaseEvent;
    using ToolManager::mouseMoveEvent;
    using ToolManager::wheelEvent;
};

#endif // TILEMAPTOOLMANAGER_H
