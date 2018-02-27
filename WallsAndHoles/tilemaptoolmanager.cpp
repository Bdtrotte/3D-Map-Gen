#include "tilemaptoolmanager.h"

inline AbstractTileMapTool *tool2TileMapTool(QSharedPointer<AbstractTool> tool)
{
    return static_cast<AbstractTileMapTool *>(tool.data());
}

TileMapToolManager::TileMapToolManager(QObject *parent)
    : ToolManager(parent) {}


QAction *TileMapToolManager::registerMapTool(QSharedPointer<AbstractTileMapTool> tool, QString name, QIcon icon,
                                             QKeySequence ks)
{
    return ToolManager::registerTool(tool, name, icon, ks);
}

void TileMapToolManager::setTileMap(TileMap *tileMap)
{
    for (AbstractToolP tool : mTools)
        tool2TileMapTool(tool)->setTileMap(tileMap);
}

void TileMapToolManager::cellActivated(int x, int y)
{
    if (!mActiveTool.isNull()) tool2TileMapTool(mActiveTool)->cellActivated(x, y);
}

void TileMapToolManager::cellClicked(int x, int y)
{
    if (!mActiveTool.isNull()) tool2TileMapTool(mActiveTool)->cellClicked(x, y);
}

void TileMapToolManager::cellReleased(int x, int y)
{
    if (!mActiveTool.isNull()) tool2TileMapTool(mActiveTool)->cellReleased(x, y);
}

void TileMapToolManager::cellHovered(int x, int y)
{
    if (!mActiveTool.isNull()) tool2TileMapTool(mActiveTool)->cellHovered(x, y);
}

void TileMapToolManager::mouseExitedMap()
{
    if (!mActiveTool.isNull()) tool2TileMapTool(mActiveTool)->mouseExitedMap();
}

void TileMapToolManager::tileTemplateChanged(TileTemplate *tileTemplate)
{
    for (AbstractToolP tool : mTools)
        tool2TileMapTool(tool)->setTileTemplate(tileTemplate);
}
