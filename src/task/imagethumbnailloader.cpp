#include "imagethumbnailloader.h"

#include <QDebug>
#include <QRect>

#define IMAGE_CACHE_MAX_SIZE (25 * 1024 * 1024)

ImageThumbnailLoader *__imageThumbnailLoader = NULL;

ImageThumbnailLoader::ImageThumbnailLoader() : QObject()
{
    _memoryCache.setMaxCost(IMAGE_CACHE_MAX_SIZE);
    _threadPool = new QThreadPool();
    _threadPool->setMaxThreadCount(3);
}

ImageThumbnailLoader::~ImageThumbnailLoader()
{
    if (_threadPool) {
        _threadPool->clear();
        delete _threadPool;
        _threadPool = nullptr;
    }
}

ImageThumbnailLoader* ImageThumbnailLoader::instance()
{
    if (__imageThumbnailLoader == NULL) {
        __imageThumbnailLoader = new ImageThumbnailLoader();
    }
    return __imageThumbnailLoader;
}

void ImageThumbnailLoader::startTask(QRunnable *task)
{
    if (this->_threadPool) {
        this->_threadPool->start(task);
    }
}

QImage ImageThumbnailLoader::getCache(QString key, QSizeF targetSize)
{
    if (!_memoryCache.contains(key)) {
        return QImage();
    }
    QImage *image = _memoryCache.object(key);
    QImage resultImage = QImage(*image);
    if (targetSize.width() != 0 && targetSize.height() != 0) {
        QImage scaledImage;
        QRect cropRect;
        if (resultImage.width() > resultImage.height()) {
            scaledImage = resultImage.scaledToHeight(targetSize.height(), Qt::SmoothTransformation);
            int surplusWidth = scaledImage.width() - targetSize.width();
            cropRect = QRect(surplusWidth / 2, 0, targetSize.width(), targetSize.height());
        } else {
            scaledImage = resultImage.scaledToWidth(targetSize.width(), Qt::SmoothTransformation);
            int surplusHeight = scaledImage.height() - targetSize.height();
            cropRect = QRect(0, surplusHeight / 2, targetSize.width(), targetSize.height());
        }
        resultImage = scaledImage.copy(cropRect);
    }
    
    return resultImage;
    
}

void ImageThumbnailLoader::saveCache(QString key, QImage image)
{
    if (_memoryCache.contains(key)) {
        return;
    }
    QImage *imagePoint = new QImage(image);
    _memoryCache.insert(key, imagePoint, imagePoint->byteCount());
}
