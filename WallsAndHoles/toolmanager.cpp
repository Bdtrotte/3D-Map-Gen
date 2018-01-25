
#include "toolmanager.h"

ToolManager::ToolManager() {
    mActionGroup = new QActionGroup(this);
}


QAction *ToolManager::registerTool(AbstractToolP tool, QString name) {

    // This automatically adds action to mActionGroup.
    QAction *action = new QAction("Activate tool: " + name, mActionGroup);
    action->setCheckable(true);


    connect(action, &QAction::toggled, this, [this, name] (bool checked) {
        if (checked)
            activateTool(name);
        else
            deactivateTool(name);
    });


    mTools.insert(name, tool);
    mToolActions.insert(name, action);

    return action;
}

QAction *ToolManager::getAction(QString name) {
    if (!mToolActions.contains(name))
        return nullptr;
    else
        return mToolActions[name];
}

void ToolManager::activateTool(QString name) {

    // Do nothing if the tool is already active.
    if (!(mTools.contains(name) && mTools[name] == mActiveTool)) {

        // Deactivate any active tool.
        if (mActiveTool != nullptr) {
            mActiveTool->deactivate();
            mActiveTool = nullptr;

            // This will invoke deactivateTool() for the corresponding tool, but deactivateTool() will not do anything.
            mActiveAction->setChecked(false);
            mActiveAction = nullptr;
        }

        // Activate a tool if a name matches.
        if (mTools.contains(name)) {

            // Activate the tool.
            mActiveTool = mTools[name];
            mActiveTool->activate();


            // Check the corresponding action. This will invoke activateTool() which will not do anything
            // since the tool is already active.
            mActiveAction = mToolActions[name];
            if (!action->isChecked())
                action->setChecked(true);

            emit toolWasActivated(tool, name);
        }
    }
}

void ToolManager::deactivateTool(QString name) {
    if (mTools.contains(name)) {
        if (mActiveTool == mTools[name]) {
            // Deactivate the tool.
            mActiveTool->deactivate();
            mActiveTool = nullptr;

            // Uncheck the action. NOTE: This will cause another deactivateTool() call with the same name,
            // but this is not a problem since the tool will not be active.
            if (mActiveAction->isChecked())
                mActiveAction->setChecked(false);

            mActiveAction = nullptr;
        }
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
