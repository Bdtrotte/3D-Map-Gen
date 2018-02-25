#ifndef TOOLMANAGER_H
#define TOOLMANAGER_H

#include "abstracttool.h"

#include <QVector>
#include <QMap>
#include <QObject>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QSharedPointer>
#include <QAction>
#include <QToolBar>

/**
 * @brief The ToolManager class represents a group of mutually exclusive tools
 * out of which only one can be active at a time.
 *
 * The object that owns the ToolManager (and therefore sends its events to the ToolManager)
 * must be the object in which the tools are used. For example, one window may contain
 * several different kind of views, including a map view. The map view should own the
 * ToolManager for rectangle / line / point brush tools because these tools only
 * make sense within the map view.
 *
 * A UI element which desires to use the ToolManager should forward mouse
 * events to the ToolManager so that they can reach the active tool.
 */
class ToolManager : public QObject {

    Q_OBJECT

public:
    using AbstractToolP = QSharedPointer<AbstractTool>;

    ToolManager(QObject *parent = nullptr);

    /**
     * @brief Registers the tool with the given identifier.
     * @param tool The tool to be registered.
     * @param name The identifier to be given to the tool.
     * @param Icon of the tool, an icon is not necessary
     * @return A QAction that, when toggled, will call either activateTool(name) or deactivateTool(name).
     */
    QAction *registerTool(AbstractToolP tool, QString name, QIcon icon = QIcon());

    /**
     * @brief Finds the action associated with the name.
     * @param name The tool name.
     * @return The action associated, or nullptr if name does not match anything.
     */
    QAction *getAction(QString name);

public slots:
    /**
     * @brief Activates the desired tool.
     * @param name The name of the tool.
     *
     * This slot can be activated directly from a UI element to enable a tool.
     */
    void activateTool(QString name);

    /**
     * @brief Deactivates the tool if it is active. NOTE: A ToolManager is exclusive,
     * so activateTool() automatically deactivates the previous tool.
     *
     * @param name The name of the tool.
     */
    void deactivateTool(QString name);
    /**
     * @brief Returns the context toolBar of the current active tool.
     */
    QToolBar *contextToolBar();


    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

signals:

    /**
     * @brief Signal emitted when a new tool is activated.
     * @param tool The tool that was activated.
     * @param name The name of the tool.
     */
    void toolWasActivated(AbstractToolP tool, QString name);

protected:
    QMap<QString, AbstractToolP> mTools;  /// A map from tool names to tool objects.
    QMap<QString, QAction*> mToolActions; /// The actions associated to each tool. All pointers delete selves when the ToolManager is destructed.

    QActionGroup *mActionGroup; /// The action group containing all of the ToolManager's actions.

    AbstractToolP mActiveTool; /// The active tool.

    QToolBar *mContextToolBar;
};

typedef QSharedPointer<ToolManager> ToolManagerP;

#endif // TOOLMANAGER_H
