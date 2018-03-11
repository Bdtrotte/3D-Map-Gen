#include "dependentundocommand.h"

#include <QDebug>

DependentUndoCommand *DependentUndoCommand::make(QVector<QObject *> dependencies, const QString &text, QUndoCommand *parent)
{
    return new DependentUndoCommand(dependencies, text, parent);
}


DependentUndoCommand::DependentUndoCommand(QVector<QObject *> dependencies, const QString &text, QUndoCommand *parent)
    : QUndoCommand(text, parent)
{
    for (QObject *dependency : dependencies) {
        if (dependency != nullptr) {
            connect(dependency, &QObject::destroyed, this, [this, dependency] () {
                // destroyed() is emitted BEFORE an object is destroyed, so dependency is still valid

                // Only do this once.
                if (isObsolete())
                    return;

                setObsolete(true);

                QString name = dependency->objectName();

                if (name.isEmpty())
                    qDebug() << "Command " << this->text() << " was made obsolete because a dependency was destroyed.";
                else
                    qDebug() << "Command " << this->text() << " was made obsolete because " << name << " was destroyed.";
            });
        }
    }
}
