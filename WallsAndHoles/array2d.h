#ifndef ARRAY2D_H
#define ARRAY2D_H

#include <QVector>
#include <QSize>
#include <QDebug>

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
    Array2D(int rows, int cols) {
        data = QVector<QVector<Type>>(rows, QVector<Type>(cols));
    }

    const Type& operator()(int r, int c) const {
        return data[r][c];
    }

    Type& operator()(int r, int c) {
        return data[r][c];
    }

    void resize(int rows, int cols)
    {
        data.resize(rows);
        for (QVector<Type> &v : data)
            v.resize(cols);
    }

    QSize size() const { return QSize(data.size(),
                                      data.empty()? 0 : data[0].size()); }

protected:
    QVector<QVector<Type>> data;
};

#endif // ARRAY2D_H
