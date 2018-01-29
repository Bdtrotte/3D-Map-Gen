#include "tilemaptoolmanager.h"

TileMapToolManager::TileMapToolManager(QObject *parent)
    : ToolManager(parent) {}

void TileMapToolManager::setTileMap(TileMap *tileMap)
{
    for (AbstractToolP t : mTools)
        static_cast<AbstractTileMapTool *>(t.data())->setTileMap(tileMap);
}

void TileMapToolManager::cellActivated(int x, int y)
{
    if (!mActiveTool.isNull()) static_cast<AbstractTileMapTool *>(mActiveTool.data())->cellActivated(x, y);
}

void TileMapToolManager::cellClicked(int x, int y)
{
    if (!mActiveTool.isNull()) static_cast<AbstractTileMapTool *>(mActiveTool.data())->cellClicked(x, y);
}

void TileMapToolManager::cellReleased(int x, int y)
{
    if (!mActiveTool.isNull()) static_cast<AbstractTileMapTool *>(mActiveTool.data())->cellReleased(x, y);
}
