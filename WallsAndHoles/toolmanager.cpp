
#include "toolmanager.h"

ToolManager::ToolManager() {

}


QAction *ToolManager::registerTool(AbstractToolP tool, QString name) {
    mTools.insert(name, tool);

    QAction *action = new QAction("Activate tool: " + name);
    connect(action, &QAction::triggered, this, [this, name]{ activateTool(name); });

    return action;
}

void ToolManager::activateTool(QString name) {

    // Deactivate any active tool.
    if (mActiveTool != nullptr) {
        mActiveTool->deactivate();
        mActiveTool = nullptr;
    }

    // Activate a tool if a name matches.
    if (mTools.contains(name)) {
        AbstractToolP tool = mTools[name];

        mActiveTool = tool;
        mActiveTool->activate();

        emit toolWasActivated(tool, name);
    }
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

void ToolManager::wheelEvent(QWheelEvent *event) {
    if (mActiveTool != nullptr)
        mActiveTool->wheelEvent(event);
}
