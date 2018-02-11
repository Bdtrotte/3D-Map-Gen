#ifndef DEREFERENCINGCONSTITERATOR_H
#define DEREFERENCINGCONSTITERATOR_H


/**
 * @brief A helper class to provide an iterator into a collection of pointers
 * such that when the iterator is dereferenced, it returns a reference instead
 * of a pointer.
 *
 * Const version of DereferncingIterator.
 */
template< typename ObjectType, typename IteratorType >
class DereferencingConstIterator
{
public:
    DereferencingConstIterator(IteratorType itr) : mItr(itr) {}


    const ObjectType &operator *()
    {
        return **mItr;
    }

    bool operator==(const DereferencingConstIterator<ObjectType, IteratorType> &other) const
    {
        return other.mItr == mItr;
    }

    bool operator!=(const DereferencingConstIterator<ObjectType, IteratorType> &other) const
    {
        return other.mItr != mItr;
    }

    DereferencingConstIterator<ObjectType, IteratorType> &operator++()
    {
        ++mItr;
        return *this;
    }

    DereferencingConstIterator<ObjectType, IteratorType> operator++(int)
    {
        return DereferencingConstIterator<ObjectType, IteratorType>(mItr++);
    }

private:
    IteratorType mItr;
};

#endif // DEREFERENCINGCONSTITERATOR_H
