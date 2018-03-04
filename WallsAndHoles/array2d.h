#ifndef ARRAY2D_H
#define ARRAY2D_H

#include <QVector>
#include <QSize>
#include <QDebug>
#include <QPoint>

#include "array2d_private.h"


// Iterator classes for Array2D.
template< typename Type > class Array2DIterator;
template< typename Type > class Array2DCIterator;

// An iterator for the neighbors of a point.
template< typename Type > class Array2DCNeighborIterator;

/**
 * @brief The Array2D class is basically a 2D implementation of QVector.
 *
 * Usage:
 *  // Construction.
 *  arr = Array2D<YourType>(rows, cols).
 *
 *  // Access.
 *  arr(r,c) = obj;
 *
 * It is possible to iterate through the Array2D using a for-each loop, as so:
 *  for (Type obj : array) { ... }
 *
 * In the above, there is no guaranteed order of traversal.
 */
template< typename Type >
class Array2D {
public:


    using IndexCollection = Array2D_Private::Array2DPointWrapper;
    using IndexedConstDataCollection = Array2D_Private::Array2DPointAndConstDataWrapper<Type>;


    Array2D() {
        // 0x0 grid
    }


    Array2D(int rows, int cols, Type defaultValue) {
        data = QVector<QVector<Type>>(rows, QVector<Type>(cols, defaultValue));
    }

    Array2D(int rows, int cols) : Array2D(rows, cols, Type()) {}

    Array2D(QSize size) : Array2D(size.width(), size.height()) {}

    const Type& operator()(int r, int c) const {
        return data[r][c];
    }

    Type& operator()(int r, int c) {
        return data[r][c];
    }

    const Type &operator ()(QPoint p) const {
        return (*this)(p.x(), p.y());
    }

    Type& operator()(QPoint p) {
        return (*this)(p.x(), p.y());
    }

    void resize(int rows, int cols)
    {
        data.resize(rows);
        for (QVector<Type> &v : data)
            v.resize(cols);
    }

    QSize size() const { return QSize(width(), height()); }

    int width() const { return data.size(); }
    int height() const { return data.empty() ? 0 : data[0].size(); }

    /**
     * @brief Returns true if (p.x(), p.y()) is a valid region.
     * @param p  The point.
     */
    bool isInBounds(QPoint p) const { return p.x() >= 0 && p.x() < width() && p.y() >= 0 && p.y() < height(); }


    /**
     * @brief indices   Returns a lightweight object for iterating over all valid (x,y) pairs.
     * @return          An object with begin() and end() methods that return iterators
     *                  that loop over valid (x,y) pairs in this array, represented as QPoints.
     */
    IndexCollection indices() const { return IndexCollection(*this); }

    /**
     * @brief indexedData   Returns a lightweight object for iterating over all data in the grid
     *                      that also provides indices.
     *
     * @return              An object with begin() and end() methods that return iterators that,
     *                      when dereferenced, return QPair<int, const Type &>.
     */
    IndexedConstDataCollection indexedData() const { return IndexedConstDataCollection(*this); }

    /* Standard begin() and end() methods. */

    Array2DCIterator<Type> begin() const {
        return Array2DCIterator<Type>::beginning(this);
    }

    Array2DCIterator<Type> end() const {
        return Array2DCIterator<Type>::ending(this);
    }


    Array2DIterator<Type> begin() {
        return Array2DIterator<Type>::beginning(this);
    }

    Array2DIterator<Type> end() {
        return Array2DIterator<Type>::ending(this);
    }

    /* Special begin() and end() methods. */

    Array2DCNeighborIterator<Type> begin_neighbors(int x, int y) const {
        return Array2DCNeighborIterator<Type>::beginning(this, x, y);
    }

    Array2DCNeighborIterator<Type> end_neighbors(int x, int y) const {
        return Array2DCNeighborIterator<Type>::ending(this, x, y);
    }

protected:
    QVector<QVector<Type>> data;
};


/* Implementations of Array2DIterator and Array2DCIterator */

template< typename Type >
class Array2DIterator {
public:
    static Array2DIterator<Type> beginning(Array2D<Type> *r) { return Array2DIterator<Type>(r, 0, 0); }
    static Array2DIterator<Type> ending(Array2D<Type> *r) { return Array2DIterator<Type>(r, 0, r->size().height()); }

    Array2DIterator(Array2D<Type> *r, int x, int y) : arr(r), x(x), y(y) {}

    bool operator==(const Array2DIterator<Type> &other) const {
        return other.arr == arr && other.x == x && other.y == y;
    }

    bool operator!=(const Array2DIterator<Type> &other) const {
        return !(*this == other);
    }

    Array2DIterator<Type> &operator++() {
        ++x;
        if (x >= arr->size().width()) {
            x = 0;
            ++y;
        }

        return *this;
    }

    // Postfix ++
    Array2DIterator<Type> operator++(int) {
        Array2DIterator<Type> iter(*this);
        ++(*this);
        return iter;
    }

    Type &operator*() {
        return (*arr)(x, y);
    }

protected:
    Array2D<Type> *arr;
    int x, y;
};

template< typename Type >
class Array2DCIterator {
public:
    static Array2DCIterator<Type> beginning(const Array2D<Type> *r) { return Array2DCIterator<Type>(r, 0, 0); }
    static Array2DCIterator<Type> ending(const Array2D<Type> *r) { return Array2DCIterator<Type>(r, 0, r->size().height()); }

    Array2DCIterator(const Array2D<Type> *r, int x, int y) : arr(r), x(x), y(y) {}

    bool operator==(const Array2DCIterator<Type> &other) const {
        return other.arr == arr && other.x == x && other.y == y;
    }

    bool operator!=(const Array2DCIterator<Type> &other) const {
        return !(*this == other);
    }

    Array2DCIterator<Type> &operator++() {
        ++x;
        if (x >= arr->size().width()) {
            x = 0;
            ++y;
        }

        return *this;
    }

    // Postfix ++
    Array2DCIterator<Type> operator++(int) {
        Array2DIterator<Type> iter(*this);
        ++(*this);
        return iter;
    }

    const Type &operator*() const {
        return (*arr)(x, y);
    }

protected:
    const Array2D<Type> *arr;
    int x, y;
};


template< typename Type >
class Array2DCNeighborIterator {
public:
    static Array2DCNeighborIterator beginning(const Array2D<Type> *r, int x, int y) { return Array2DCNeighborIterator(r, x, y, 0); }
    static Array2DCNeighborIterator ending(const Array2D<Type> *r, int x, int y) { return Array2DCNeighborIterator(r, x, y, 7); }


    Array2DCNeighborIterator(const Array2D<Type> *r, int x, int y, int idx) : arr(r), centerX(x), centerY(y), neighborIndex(idx) {
        // Make sure neighborIndex starts within bounds and doesn't go beyond 7.
        while (!isInBounds() && neighborIndex < 7)
            ++neighborIndex;
    }

    bool operator==(const Array2DCNeighborIterator<Type> &other) const {
        return other.arr == arr && other.centerX == centerX && other.centerY == centerY && other.neighborIndex == neighborIndex;
    }

    bool operator!=(const Array2DCNeighborIterator<Type> &other) const {
        return !(*this == other);
    }

    Array2DCNeighborIterator<Type> &operator++() {
        ++neighborIndex;

        // Make sure neighborIndex is within bounds and not above 7.
        while (!isInBounds() && neighborIndex < 7)
            ++neighborIndex;

        return *this;
    }

    // Postfix ++
    Array2DCNeighborIterator<Type> operator++(int) {
        Array2DIterator<Type> iter(*this);
        ++(*this);
        return iter;
    }

    const Type &operator*() const {
        // assumes isInBounds() is true
        return (*arr)(getX(neighborIndex), getY(neighborIndex));
    }

protected:

    // Helper functions to map neighbor indices to positions.
    // The mapping is:
    //  0 1 2
    //  3   4
    //  5 6 7
    // with the top-left corner having the smallest x and y coordinates.

    int getX(int neighbor) const {
        if (neighbor == 0 || neighbor == 3 || neighbor == 5)
            return centerX - 1;
        if (neighbor == 1 || neighbor == 6)
            return centerX;
        return centerX + 1;
    }

    int getY(int neighbor) const {
        if (neighbor < 3)
            return centerY - 1;
        if (neighbor < 5)
            return centerY;
        return centerY + 1;
    }

    bool isInBounds() const {
        int wdth = arr->width();
        int hght = arr->height();

        int x = getX(neighborIndex);
        int y = getY(neighborIndex);

        return 0 <= x && x < wdth
                && 0 <= y && y < hght;
    }


    const Array2D<Type> *arr;
    int centerX, centerY;
    int neighborIndex;
};


#endif // ARRAY2D_H
