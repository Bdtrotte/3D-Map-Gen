#ifndef DEREFERENCINGITERATOR_H
#define DEREFERENCINGITERATOR_H


/**
 * @brief A helper class to provide an iterator into a collection of pointers
 * such that when the iterator is dereferenced, it returns a reference instead
 * of a pointer.
 */
template< typename ObjectType, typename IteratorType >
class DereferencingIterator
{
public:
    DereferencingIterator(IteratorType itr) : mItr(itr) {}


    ObjectType &operator *()
    {
        return **mItr;
    }

    bool operator==(const DereferencingIterator<ObjectType, IteratorType> &other) const
    {
        return other.mItr == mItr;
    }

    bool operator!=(const DereferencingIterator<ObjectType, IteratorType> &other) const
    {
        return other.mItr != mItr;
    }

    DereferencingIterator<ObjectType, IteratorType> &operator++()
    {
        ++mItr;
        return *this;
    }

    DereferencingIterator<ObjectType, IteratorType> operator++(int)
    {
        return DereferencingIterator<ObjectType, IteratorType>(mItr++);
    }

private:
    IteratorType mItr;
};

#endif // DEREFERENCINGITERATOR_H
