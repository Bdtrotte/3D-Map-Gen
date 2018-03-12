#ifndef UNDOCOMMANDFROMACTION_H
#define UNDOCOMMANDFROMACTION_H

#include <QUndoCommand>

// For std::function
#include <functional>

/**
 * @brief Creates an undo command that invokes the provided functions on undo() and redo().
 *
 * This is helpful when the undo / redo code already exists as a method for some other object.
 */
class UndoCommandFromFunctions : public QUndoCommand
{
public:
    /**
     * @brief performCommand    Creates the command and performs it immediately.
     * @param redo              The function to call on redo.
     * @param undo              The function to call on undo.
     */
    template<typename Func1, typename Func2>
    static UndoCommandFromFunctions *make(Func1 redo, Func2 undo, const QString &text = "", QUndoCommand *parent = nullptr)
    {
        UndoCommandFromFunctions *cmd = new UndoCommandFromFunctions(redo, undo, text, parent);

        return cmd;
    }

    void redo() override
    {
        mRedo();
    }

    void undo() override
    {
        mUndo();
    }

private:

    UndoCommandFromFunctions(std::function<void()> redo, std::function<void()> undo, const QString &text = "", QUndoCommand *parent = nullptr)
        : QUndoCommand(text, parent)
        , mRedo(redo)
        , mUndo(undo) {}

    std::function<void()> mRedo;
    std::function<void()> mUndo;
};

#endif // UNDOCOMMANDFROMACTION_H
