#include "toolmanager.h"

ToolManager::ToolManager() {

}


void ToolManager::registerTool(ToolP tool, QString name) {
    mTools.insert(name, tool);
}

void ToolManager::activateTool(QString name) {
    ToolP tool = mTools[name];

    if (mActiveTool != nullptr)
        mActiveTool->deactivate();
    mActiveTool = tool;
    mActiveTool->activate();

    emit toolWasActivated(tool, name);
}



void ToolManager::mousePressEvent(QMouseEvent *event) {
    if (mActiveTool != nullptr)
        mActiveTool->mousePressEvent(event);
}

void ToolManager::mouseReleaseEvent(QMouseEvent *event) {
    if (mActiveTool != nullptr)
        mActiveTool->mouseReleaseEvent(event);
}
void ToolManager::mouseMoveEvent(QMouseEvent *event) {
    if (mActiveTool != nullptr)
        mActiveTool->mouseMoveEvent(event);
}
