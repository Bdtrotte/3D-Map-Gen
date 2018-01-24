#ifndef TOOLMANAGER_H
#define TOOLMANAGER_H

#include <QVector>
#include <QMap>
#include <QObject>
#include <QMouseEvent>
#include <QSharedPointer>
#include "tool.h"

/**
 * @brief The ToolManager class represents a group of mutually exclusive tools
 * out of which only one can be active at a time.
 *
 * A UI element which desires to use the ToolManager should forward mouse
 * events to the ToolManager so that they can reach the active tool.
 */
class ToolManager : public QObject {

    Q_OBJECT

public:
    using ToolP = QSharedPointer<Tool>;

    ToolManager();



public slots:
    /**
     * @brief Registers the tool with the given identifier.
     * @param tool The tool to be registered.
     * @param name The identifier to be given to the tool.
     */
    void registerTool(ToolP tool, QString name);

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

signals:

    /**
     * @brief Signal emitted when a new tool is activated.
     * @param tool The tool that was activated.
     * @param name The name of the tool.
     */
    void toolWasActivated(ToolP tool, QString name);

protected:
    QMap<QString, ToolP> mTools;

    ToolP mActiveTool;
};

#endif // TOOLMANAGER_H
