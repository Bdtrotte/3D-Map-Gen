#ifndef TOOLMANAGER_H
#define TOOLMANAGER_H

#include <QVector>
#include <QMap>
#include <QObject>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QSharedPointer>
#include <QAction>
#include "abstracttool.h"

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

    ToolManager();


    /**
     * @brief Registers the tool with the given identifier.
     * @param tool The tool to be registered.
     * @param name The identifier to be given to the tool.
     * @return A QAction that, when triggered, will call activateTool(name).
     */
    QAction *registerTool(AbstractToolP tool, QString name);


public slots:
    /**
     * @brief Activates the desired tool.
     * @param name The name of the tool.
     *
     * This slot can be activated directly from a UI element to enable a tool.
     */
    void activateTool(QString name);


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
    QMap<QString, AbstractToolP> mTools;

    AbstractToolP mActiveTool;
};

typedef QSharedPointer<ToolManager> ToolManagerP;

#endif // TOOLMANAGER_H
