#include "emptyparentcommand.h"

EmptyParentCommand *EmptyParentCommand::make(const QString &text, QUndoCommand *parent)
{
    return new EmptyParentCommand(text, parent);
}


void EmptyParentCommand::redo()
{
    if (checkIfChildrenObsolete())
        setObsolete(true);
    else
        QUndoCommand::redo();
}


void EmptyParentCommand::undo()
{
    if (checkIfChildrenObsolete())
        setObsolete(true);
    else
        QUndoCommand::undo();
}


bool EmptyParentCommand::checkIfChildrenObsolete() const
{
    for (int idx = 0; idx < childCount(); ++idx)
        if (!child(idx)->isObsolete())
            return false;

    return true;
}


EmptyParentCommand::EmptyParentCommand(const QString &text, QUndoCommand *parent)
    : QUndoCommand(text, parent) {}
