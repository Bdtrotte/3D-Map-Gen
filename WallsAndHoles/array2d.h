#ifndef ARRAY2D_H
#define ARRAY2D_H

#include <QVector>
#include <QSize>


template< typename Type > class Array2DIterator;

/**
 * @brief The Array2D class is basically a 2D implementation of QVector.
 *
 * Usage:
 *  // Construction.
 *  arr = Array2D<YourType>(rows, cols).
 *
 *  // Access.
 *  arr(r,c) = obj;
 */
template< typename Type >
class Array2D {
public:
    Array2D() {
        // 0x0 grid
    }


    Array2D(int rows, int cols) {
        data = QVector<QVector<Type>>(rows, QVector<Type>(cols));
    }

    Array2D(QSize size) : Array2D(size.width(), size.height()) {}

    const Type& operator()(int r, int c) const {
        return data[r][c];
    }

    Type& operator()(int r, int c) {
        return data[r][c];
    }

    void resize(int rows, int cols)
    {
        data.resize(rows);
        for (QVector<Type> v : data)
            v.resize(cols);
    }

    QSize size() const { return QSize(data.size(),
                                      data.empty()? 0 : data[0].size()); }


    Array2DIterator<Type> begin() {
        return Array2DIterator(this, 0, 0);
    }

    Array2DIterator<Type> end() {
        auto sz = size();
        int width = sz.width();
        int height = sz.height();
        return Array2DIterator(this, width, height);
    }

protected:
    QVector<QVector<Type>> data;
};


template< typename Type >
class Array2DIterator {
public:
    Array2DIterator(Array2D *r, int x, int y) : arr(r), x(x), y(y) {}

    bool operator==(const Array2DIterator &other) const {
        return other.arr == arr && other.x == x && other.y == y;
    }

    bool operator!=(const Array2DIterator &other) const {
        return !(*this == other);
    }

    Array2DIterator &operator++() {
        ++x;
        if (x >= arr->size().width()) {
            x = 0;
            ++y;
        }

        return *this;
    }

    // Postfix ++
    Array2DIterator operator++(int) {
        Array2DIterator iter(*this);
        ++(*this);
        return iter;
    }

    Type &operator*() {
        return (*arr)(x, y);
    }

protected:
    Array2D *arr;
    int x, y;
};


#endif // ARRAY2D_H
