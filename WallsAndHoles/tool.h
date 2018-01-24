#ifndef TOOL_H
#define TOOL_H

#include <QMouseEvent>

/**
 * @brief The Tool class represents a UI tool. A UI tool can be activated or
 * deactivated.
 *
 * The purpose of the Tool class is to separate a tool's implementation from the
 * implementation of the context in which it is used (e.g. separate the code
 * of the brush tool from the grid rendering widget).
 *
 * It is assumed that tools are used with a mouse. A tool can expect to receive
 * mouse press, release, and move events from whatever widget it is assigned to.
 *
 * A constructor of a subclass of Tool should receive whatever data it needs
 * to function. A Brush tool should have some way of accessing the TileMap
 * and some way of determining the visual parameters of the grid.
 */
class Tool {
public:

    virtual ~Tool() {}


    /**
     * @brief Activates the tool. This is called before the tool is used.
     */
    virtual void activate() = 0;

    /**
     * @brief Deactivates the tool. This is called before the tool stops being used.
     */
    virtual void deactivate() = 0;


    virtual void mousePressEvent(QMouseEvent *) {}
    virtual void mouseReleaseEvent(QMouseEvent *) {}
    virtual void mouseMoveEvent(QMouseEvent *) {}

};

#endif // TOOL_H
