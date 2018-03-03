#ifndef ARRAY2D_PRIVATE_H
#define ARRAY2D_PRIVATE_H

#include <QPoint>
#include <QPair>

#include "array2d.h"


template< typename Type > class Array2D;

/*
 * This header contains some helper classes and methods for Array2D that are
 * not necessary for users of Array2D to know.
 *
 * The purpose is to reduce clutter in "array2d.h" and in the global namespace.
 * */

namespace Array2D_Private {


/**
 * @brief An almost-implemented iterator object that iterates through all positions in a rectangle.
 *
 * A postfix ++ is not implemented (since this requires copying self).
 *
 * Implementing operator*() will make this a valid iterator.
 */
struct Array2DItrTemplate {

    Array2DItrTemplate &operator++()
    {
        ++mY;
        if (mY == mHeight) {
            mY = 0;
            ++mX;
        }

        return *this;
    }

    bool operator ==(const Array2DItrTemplate &other) const
    {
        return mX == other.mX && mY == other.mY && mWidth == other.mWidth && mHeight == other.mHeight;
    }

    bool operator !=(const Array2DItrTemplate &other) const
    {
        return !(*this == other);
    }

protected:
    int x() const { return mX; }
    int y() const { return mY; }

    int width() const { return mWidth; }
    int height() const { return mHeight; }

    Array2DItrTemplate(int w, int h, int x, int y)
        : mWidth(w), mHeight(h)
        , mX(x), mY(y) {}

private:

    int mWidth;
    int mHeight;
    int mX;
    int mY;
};


/**
 * @brief A lightweight class to allow looping through all the valid indices in Array2D.
 */
class Array2DPointWrapper {
public:
    struct Itr;

    template< typename T >
    Array2DPointWrapper(const Array2D<T> &arr)
        : mWidth(arr.width()), mHeight(arr.height())
    {}


    Itr begin() const { return Itr::start(mWidth, mHeight); }
    Itr end() const { return Itr::end(mWidth, mHeight); }


    struct Itr : public Array2DItrTemplate {
        static Itr start(int w, int h) { return Itr(w, h, 0, 0); }
        static Itr end(int w, int h)   { return Itr(w, h, w, 0); }

        QPoint operator *() const
        {
            return QPoint(x(), y());
        }

    private:
        Itr(int w, int h, int x, int y)
            : Array2DItrTemplate(w, h, x, y) {}
    };


protected:
    int mWidth;
    int mHeight;
};

/**
 * @brief A lightweight class to allow looping through all the data in an Array2D, outputting
 * QPairs of (position, const data &).
 */
template< typename Type >
class Array2DPointAndConstDataWrapper {
public:
    struct Itr;

    Array2DPointAndConstDataWrapper(const Array2D<Type> &arr)
        : mArray(arr)
    {}


    Itr begin() const { return Itr::start(mArray); }
    Itr end() const { return Itr::end(mArray); }


    struct Itr : public Array2DItrTemplate {
        static Itr start(const Array2D<Type> &arr) { return Itr(arr, 0, 0); }
        static Itr end(const Array2D<Type> &arr)   { return Itr(arr, arr.width(), 0); }

        QPair<QPoint, const Type &> operator *() const
        {
            QPoint pt = QPoint(x(), y());
            return QPair<QPoint, const Type &> (pt, mArray(pt));
        }

    private:
        Itr(const Array2D<Type> arr, int x, int y)
            : Array2DItrTemplate(arr.width(), arr.height(), x, y)
            , mArray(arr)
        {}

        const Array2D<Type> &mArray;
    };


protected:
    const Array2D<Type> &mArray;
};



}


#endif // ARRAY2D_PRIVATE_H
