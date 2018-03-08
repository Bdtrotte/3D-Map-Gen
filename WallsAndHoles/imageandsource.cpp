#include "imageandsource.h"

QMap<QString, SharedImageAndSource> ImageAndSource::mLoadedImages;

SharedImageAndSource ImageAndSource::getSharedImageAndSource(QString filePath)
{
    auto itr = mLoadedImages.find(filePath);
    if (itr != mLoadedImages.end())
        return *itr;

    return *mLoadedImages.insert(filePath, SharedImageAndSource(new ImageAndSource(filePath)));
}
