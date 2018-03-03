#ifndef IMAGEANDSOURCE_H
#define IMAGEANDSOURCE_H

#include <QSharedPointer>
#include <QImage>

class ImageAndSource;

typedef QSharedPointer<ImageAndSource> SharedImageAndSource;

class ImageAndSource
{
public:
    static SharedImageAndSource getSharedImageAndSource(QString filePath)
    {
        return SharedImageAndSource(new ImageAndSource(filePath));
    }

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
};

#endif // IMAGEANDSOURCE_H
