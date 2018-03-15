#include "toolmanager.h"

#include <QSpinBox>
#include <QWidgetAction>
#include <QColorDialog>

ToolManager::ToolManager(QObject *parent)
    : QObject(parent)
    , mActionGroup(new QActionGroup(this))
    , mActiveTool(nullptr)
    , mContextToolBar(new QToolBar())
    , mNoTool(new QAction(mActionGroup))
{
    mNoTool->setCheckable(true);
}


QAction *ToolManager::registerTool(AbstractTool *tool, QString name, QIcon icon, QKeySequence ks)
{
    tool->setParent(this);

    // This automatically adds action to mActionGroup.
    QAction *action = new QAction(name, mActionGroup);

    if(!icon.isNull()){
        action->setIcon(icon);
    }

    if(!ks.isEmpty()){
        action->setShortcut(ks);
        action->setToolTip(name+" ("+ks.toString()+ ")");
    }

    action->setCheckable(true);

    connect(action, &QAction::toggled,
            this, [this, name] (bool checked) {
        if (checked)
            activateTool(name);
        else
            deactivateTool(name);
    });


    mTools.insert(name, tool);
    mToolActions.insert(name, action);

    return action;
}

QAction *ToolManager::getAction(QString name)
{
    if (!mToolActions.contains(name))
        return nullptr;
    else
        return mToolActions[name];
}

void ToolManager::activateTool(QString name)
{
    //Shouldn't call this with a name not contained.
    Q_ASSERT(mTools.contains(name));

    auto tool = mTools[name];

    if (tool == mActiveTool) return;

    if (mActiveTool != nullptr)
        mContextToolBar->clear();

    mActiveTool = tool;
    mActiveTool->activate();

    //fill context Tool bar with tools context actions.
    mContextToolBar->addActions(mActiveTool->contextActions());

    emit toolWasActivated(mActiveTool, name);
}

void ToolManager::deactivateTool(QString name)
{
    //Shouldn't call this with a name not contained.
    Q_ASSERT(mTools.contains(name));

    auto tool = mTools[name];

    tool->deactivate();

    if (mActiveTool == tool) {
        mActiveTool = nullptr;

        //Clears the context toolBar of the tool being deactivated
        mContextToolBar->clear();
    }
}

QToolBar *ToolManager::contextToolBar()
{
    return mContextToolBar;
}

void ToolManager::mousePressEvent(QMouseEvent *event)
{
    if (mActiveTool != nullptr)
        mActiveTool->mousePressEvent(event);
}

void ToolManager::mouseReleaseEvent(QMouseEvent *event)
{
    if (mActiveTool != nullptr)
        mActiveTool->mouseReleaseEvent(event);
}

void ToolManager::mouseMoveEvent(QMouseEvent *event)
{
    if (mActiveTool != nullptr)
        mActiveTool->mouseMoveEvent(event);
}

void ToolManager::wheelEvent(QWheelEvent *event)
{
    if (mActiveTool != nullptr)
        mActiveTool->wheelEvent(event);
}
