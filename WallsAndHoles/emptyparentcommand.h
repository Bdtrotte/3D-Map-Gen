#ifndef EMPTYPARENTCOMMAND_H
#define EMPTYPARENTCOMMAND_H

#include <QUndoCommand>

/**
 * @brief The EmptyParentCommand class works almost exactly like the default
 * QUndoCommand implementation, except that it sets itself obsolete if all
 * of its children are obsolete. This way, command macros that do nothing
 * will get removed.
 */
class EmptyParentCommand : public QUndoCommand
{
public:

    /**
     * @brief make      Makes an empty parent command with the given text.
     * @param text      The text of the command.
     * @param parent    An optional parent command.
     * @return
     */
    static EmptyParentCommand *make(const QString &text, QUndoCommand *parent = nullptr);


    void redo() override;
    void undo() override;

private:

    bool checkIfChildrenObsolete() const;

    EmptyParentCommand(const QString &text, QUndoCommand *parent);
};

#endif // EMPTYPARENTCOMMAND_H
