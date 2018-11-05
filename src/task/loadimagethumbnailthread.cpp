#include "loadimagethumbnailthread.h"

#include "../task/imagethumbnailloader.h"

#ifdef Q_OS_IOS
#include "../utils/iosassetutils.h"
#endif

LoadImageThumbnailThread::LoadImageThumbnailThread(QString source, QSizeF targetSize, QObject *parent) : QObject(parent), QRunnable()
{
    this->_source = source;
    this->_targetSize = targetSize;
}

void LoadImageThumbnailThread::run()
{
    QImage image;
    bool result = false;
#ifdef Q_OS_ANDROID
    result = image.load(_source);
#endif
#ifdef Q_OS_IOS
    IOSAssetUtils utils;
    image = utils.getThumbnail(_source);
    result = !image.isNull();
#endif
    if (result) {
        QImage saveImage = image;
        if (image.width() > image.height() && image.width() > 200) {
            saveImage = image.scaledToWidth(200, Qt::SmoothTransformation);
        } else if (image.height() > image.width() && image.height() > 200) {
            saveImage = image.scaledToHeight(200, Qt::SmoothTransformation);
        }
        ImageThumbnailLoader::instance()->saveCache(_source, saveImage);
        
        QImage resultImage = saveImage;
        
        QImage scaledImage;
        QRect cropRect;
        if (resultImage.width() > resultImage.height()) {
            scaledImage = resultImage.scaledToHeight(_targetSize.height(), Qt::SmoothTransformation);
            int surplusWidth = scaledImage.width() - _targetSize.width();
            cropRect = QRect(surplusWidth / 2, 0, _targetSize.width(), _targetSize.height());
        } else {
            scaledImage = resultImage.scaledToWidth(_targetSize.width(), Qt::SmoothTransformation);
            int surplusHeight = scaledImage.height() - _targetSize.height();
            cropRect = QRect(0, surplusHeight / 2, _targetSize.width(), _targetSize.height());
        }
        resultImage = scaledImage.copy(cropRect);
        emit signal_done(_source, resultImage);
    } else {
        emit signal_done(_source, image);
    }
}
