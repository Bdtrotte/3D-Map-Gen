#ifndef IMAGEANDSOURCE_H
#define IMAGEANDSOURCE_H

#include <QSharedPointer>
#include <QImage>
#include <QMap>

class ImageAndSource;

typedef QSharedPointer<ImageAndSource> SharedImageAndSource;

class ImageAndSource
{
public:
    static SharedImageAndSource getSharedImageAndSource(QString filePath);

    // TODO: This should probably return a const ref instead
    QSharedPointer<QImage> image() { return mImage; }
    const QString &source() const { return mSource; }

private:
    ImageAndSource(QString filePath)
        : mSource(filePath)
        , mImage(QSharedPointer<QImage>::create(filePath))
    {
        if (mImage->isNull())
            mSource = "";
    }

    QString mSource;
    QSharedPointer<QImage> mImage;

    static QMap<QString, SharedImageAndSource> mLoadedImages;
};

#endif // IMAGEANDSOURCE_H
