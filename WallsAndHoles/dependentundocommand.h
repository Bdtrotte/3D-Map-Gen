#ifndef DEPENDENTUNDOCOMMAND_H
#define DEPENDENTUNDOCOMMAND_H

#include <QUndoCommand>
#include <QObject>
#include <QVector>

/**
 * @brief The DependentUndoCommand class is used to make an empty undo command
 * that obsoletes itself when any of the dependencies are destructed. This is
 * used to help avoid crashes.
 *
 * Make this command the parent of any other command.
 */
class DependentUndoCommand : public QObject, public QUndoCommand
{

    Q_OBJECT

public:

    static DependentUndoCommand *make(QVector<QObject *> dependencies, const QString &text = "", QUndoCommand *parent = nullptr);

private:

    DependentUndoCommand(QVector<QObject *> dependencies, const QString &text = "", QUndoCommand *parent = nullptr);

};

#endif // DEPENDENTUNDOCOMMAND_H
