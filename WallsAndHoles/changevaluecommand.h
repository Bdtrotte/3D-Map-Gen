#ifndef CHANGEVALUECOMMAND_H
#define CHANGEVALUECOMMAND_H

#include <functional>
#include <QUndoCommand>


template< typename ValueType >
class ChangeValueCommand : public QUndoCommand
{
public:



    /**
     * @brief make              Creates a change-value command.
     * @param variable          The pointer to the variable that is modified.
     * @param newValue          The variable's new value.
     * @param oldValue          The variable's old value (sometimes the pre-action value doesn't match the current value).
     * @param text              A short name or description (see QUndoCommand documentation).
     * @param postChangeFunc    Action to perform after changing the variable. Should have no parameters and no return value.
     * @param parent            The parent of the command.
     * @return                  The command that was performed.
     */
    template< typename Func >
    static ChangeValueCommand *make(
            ValueType *variable,
            ValueType newValue,
            ValueType oldValue,
            const QString &text, // "change value" is not a useful default text (since the user can't see the code)
            Func postChangeFunc,
            QUndoCommand *parent = nullptr)
    {
        std::function<void()> func = postChangeFunc;

        return new ChangeValueCommand(variable, oldValue, newValue, text, func, parent);
    }


    /**
     * @brief make              Creates a change-value command.
     * @param variable          The pointer to the variable that is modified.
     * @param newValue          The variable's new value.
     * @param text              A short name or description (see QUndoCommand documentation).
     * @param postChangeFunc    Action to perform after changing the variable. Should have no parameters and no return value.
     * @param parent            The parent of the command.
     * @return                  The command that was performed.
     */
    template< typename Func >
    static ChangeValueCommand *make(
            ValueType *variable,
            ValueType newValue,
            const QString &text, // "change value" is not a useful default text (since the user can't see the code)
            Func postChangeFunc,
            QUndoCommand *parent = nullptr)
    {
        ValueType oldValue = *variable;
        return make(variable, newValue, oldValue, text, postChangeFunc, parent);
    }

    /**
     * @brief make              Creates a change-value command.
     * @param variable          The pointer to the variable that is modified.
     * @param newValue          The variable's new value.
     * @param text              A short name or description (see QUndoCommand documentation).
     * @param parent            The parent of the command.
     * @return                  The command that was performed.
     */
    static ChangeValueCommand *make(
            ValueType *variable,
            ValueType newValue,
            const QString &text,
            QUndoCommand *parent = nullptr)
    {
        return make(variable, newValue, text, [](){}, parent);
        //                  isn't this beautiful ^
    }




    void redo() override
    {
        *mVal = mNewVal;
        mPostChangeFunc();
    }

    void undo() override
    {
        *mVal = mOldVal;
        mPostChangeFunc();
    }

private:

    ChangeValueCommand(ValueType *variable,
                       ValueType oldValue,
                       ValueType newValue,
                       const QString &text,
                       std::function<void()> postChange,
                       QUndoCommand *parent)
        : QUndoCommand(text, parent)
        , mVal(variable)
        , mOldVal(oldValue)
        , mNewVal(newValue)
        , mPostChangeFunc(postChange) {}

    ValueType *mVal;

    ValueType mOldVal;
    ValueType mNewVal;

    std::function<void()> mPostChangeFunc;
};

#endif // CHANGEVALUECOMMAND_H
