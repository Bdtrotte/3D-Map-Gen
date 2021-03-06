#ifndef TOOL_H
#define TOOL_H

#include <QMouseEvent>
#include <QWheelEvent>
#include <QAction>
#include <QObject>

/**
 * @brief The AbstractTool class represents a UI tool. A UI tool can be activated or
 * deactivated.
 *
 * The purpose of the AbstractTool class is to separate a tool's implementation from the
 * implementation of the context in which it is used (e.g. separate the code
 * of the brush tool from the grid rendering widget).
 *
 * It is assumed that tools are used with a mouse. A tool can expect to receive
 * mouse press, release, and move events from whatever widget it is assigned to.
 *
 * A constructor of a subclass of AbstractTool should receive whatever data it needs
 * to function. A Brush tool should have some way of accessing the TileMap
 * and some way of determining the visual parameters of the grid.
 */
class AbstractTool : public QObject
{
public:
    virtual ~AbstractTool() {}


    /**
     * @brief Activates the tool. This is called before the tool is used.
     */
    virtual void activate() {}

    /**
     * @brief Deactivates the tool. This is called before the tool stops being used.
     */
    virtual void deactivate() {}

    /**
     * @brief Returns a vector of qactions which can be used to modify the behaviour of the tool.
     * Will be used by the tool manager to add the actions to a context tool bar.
     */
    virtual QList<QAction *> contextActions() { return {}; }


    virtual void mousePressEvent(QMouseEvent *) {}
    virtual void mouseReleaseEvent(QMouseEvent *) {}
    virtual void mouseMoveEvent(QMouseEvent *) {}
    virtual void wheelEvent(QWheelEvent *) {}
};

#endif // TOOL_H
