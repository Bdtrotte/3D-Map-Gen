#ifndef CHANGEVALUECOMMAND_H
#define CHANGEVALUECOMMAND_H

#include <functional>
#include <QUndoCommand>


template< typename ValueType >
class ChangeValueCommand : public QUndoCommand
{
public:



    /**
     * @brief performCommand    Creates and immediately performs a change-value command.
     * @param variable          The pointer to the variable that is modified.
     * @param newValue          The variable's new value.
     * @param oldValue          The variable's old value (sometimes the pre-action value doesn't match the current value).
     * @param text              A short name or description (see QUndoCommand documentation).
     * @param postChangeFunc    Action to perform after changing the variable. Should have no parameters and no return value.
     * @return                  The command that was performed.
     */
    template< typename Func >
    static ChangeValueCommand *performCommand(
            ValueType *variable,
            ValueType newValue,
            ValueType oldValue,
            const QString &text, // "change value" is not a useful default text (since the user can't see the code)
            Func postChangeFunc)
    {
        std::function<void()> func = postChangeFunc;

        ChangeValueCommand *cmd = new ChangeValueCommand(variable, oldValue, newValue, text, func);

        cmd->redo();

        return cmd;
    }


    /**
     * @brief performCommand    Creates and immediately performs a change-value command.
     * @param variable          The pointer to the variable that is modified.
     * @param newValue          The variable's new value.
     * @param text              A short name or description (see QUndoCommand documentation).
     * @param postChangeFunc    Action to perform after changing the variable. Should have no parameters and no return value.
     * @return                  The command that was performed.
     */
    template< typename Func >
    static ChangeValueCommand *performCommand(
            ValueType *variable,
            ValueType newValue,
            const QString &text, // "change value" is not a useful default text (since the user can't see the code)
            Func postChangeFunc)
    {
        ValueType oldValue = *variable;
        return performCommand(variable, newValue, oldValue, text, postChangeFunc);
    }

    /**
     * @brief performCommand    Creates and immediately performs a change-value command.
     * @param variable          The pointer to the variable that is modified.
     * @param newValue          The variable's new value.
     * @param text              A short name or description (see QUndoCommand documentation).
     * @return                  The command that was performed.
     */
    static ChangeValueCommand *performCommand(
            ValueType *variable,
            ValueType newValue,
            const QString &text)
    {
        return performCommand(variable, newValue, text, [](){});
        //                           isn't this beautiful ^
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

    ChangeValueCommand(ValueType *variable, ValueType oldValue, ValueType newValue, const QString &text, std::function<void()> postChange)
        : QUndoCommand(text)
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
