#include "photoview.h"

#include <QPainter>
#include <QImage>
#include <QDebug>
#include <QDateTime>

#include "../utils/utils.h"
#include "../task/loadimagedatathread.h"

PhotoView::PhotoView(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    _cropAreaInited = false;
    _maxScale = 3;
}

void PhotoView::initCropArea()
{
    QRectF viewRect = boundingRect();
    
    int cropWidth;
    int cropHeight;
    int cropX;
    int cropY;
    if (viewRect.width() < viewRect.height()) {
        cropWidth = viewRect.width() / 4 * 3;
        cropHeight = cropWidth;
    } else {
        cropHeight = viewRect.height() / 4 * 3;
        cropWidth = cropHeight;
    }
    cropX = (viewRect.width() - cropWidth) / 2;
    cropY = (viewRect.height()- cropHeight) / 2;
    
    _cropLeft = cropX;
    _cropTop = cropY;
    _cropRight = cropX + cropWidth;
    _cropBottom = cropY + cropHeight;
    _cropWidth = cropWidth;
    _cropHeight = cropHeight;
    
    _cropAreaInited = true;
    emit signal_cropAreaInited();
}

void PhotoView::setSrc(QString path)
{
    if (path.compare(_path) == 0) {
        return;
    }
    
    emit signal_loadingStarted();
    
    _path = path;
    
    LoadImageDataThread *thread = new LoadImageDataThread(_path, this);
    connect(thread, SIGNAL(signal_done(QString,QImage)), SLOT(slot_loadImageDataDone(QString,QImage)));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

void PhotoView::moveBegan()
{
    _moveTouchPoint = QPointF(_drawImageRect.x(), _drawImageRect.y());
}

void PhotoView::moveUpdate(int offseetX, int offsetY)
{
    int drawImageX = _moveTouchPoint.x();
    int drawImageY = _moveTouchPoint.y();
    int drawImageWidth = _drawImageRect.width();
    int drawImageHeight = _drawImageRect.height();
    
    int afterX = drawImageX + offseetX;
    if (afterX > _cropLeft) {
        drawImageX = _cropLeft;
    } else if (afterX + drawImageWidth <= _cropRight) {
        drawImageX = _cropRight - drawImageWidth;
    } else {
        drawImageX = afterX;
    }

    int afterY = drawImageY + offsetY;
    if (afterY > _cropTop) {
        drawImageY = _cropTop;
    } else if (afterY + drawImageHeight <= _cropBottom) {
        drawImageY = _cropBottom - drawImageHeight;
    } else {
        drawImageY = afterY;
    }
    
    _drawImageRect = QRectF(drawImageX, drawImageY, drawImageWidth, drawImageHeight);
    
    this->update();
}

void PhotoView::moveEnded()
{
    _moveTouchPoint = QPointF(_drawImageRect.x(), _drawImageRect.y());
}

void PhotoView::scaleBegan(int centerX, int centerY)
{
    _scaleCenterPoint = QPointF(centerX, centerY);
    _scaleSourceRect = QRectF(_drawImageRect.x(), _drawImageRect.y(), _drawImageRect.width(), _drawImageRect.height());
}

void PhotoView::scaleUpdate(float scale)
{
    int sourceX = _scaleSourceRect.x();
    int sourceY = _scaleSourceRect.y();
    int sourceWidth = _scaleSourceRect.width();
    int sourceHeight = _scaleSourceRect.height();
    
    int afterWidth = sourceWidth * scale;
    int afterHeight = sourceHeight * scale;
    int afterX = sourceX;
    int afterY = sourceY;
    
    if (_sourceImage.width() < _sourceImage.height()) {
        if (afterWidth > _cropWidth * _maxScale) {
            afterWidth = _cropWidth * _maxScale;
            afterHeight = ((double)(_cropWidth * _maxScale) / (double)_sourceImage.width()) * _sourceImage.height();
        } else if (afterWidth < _cropWidth) {
            afterWidth = _cropWidth;
            afterHeight = ((double)_cropWidth / (double)_sourceImage.width()) * _sourceImage.height();
        }
    } else {
        if (afterHeight > _cropHeight * _maxScale) {
            afterHeight = _cropHeight * _maxScale;
            afterWidth = ((double)(_cropHeight * _maxScale) / (double)_sourceImage.height()) * _sourceImage.width();
        } else if (afterHeight < _cropHeight) {
            afterHeight = _cropHeight;
            afterWidth = ((double)_cropHeight / (double)_sourceImage.height()) * _sourceImage.width();
        }
    }
    
    if (_scaleCenterPoint.x() < _cropLeft) {
        _scaleCenterPoint.setX(_cropLeft);
    } else if (_scaleCenterPoint.x() > _cropRight) {
        _scaleCenterPoint.setX(_cropRight);
    }
    if (_scaleCenterPoint.y() < _cropTop) {
        _scaleCenterPoint.setY(_cropTop);
    } else if (_scaleCenterPoint.y() > _cropBottom) {
        _scaleCenterPoint.setY(_cropBottom);
    }
    
    float centerXPercent = (float)(_scaleCenterPoint.x() - sourceX) / (float)sourceWidth;
    float centerYPercent = (float)(_scaleCenterPoint.y() - sourceY) / (float)sourceHeight;
    
    int afterCenterX = afterWidth * centerXPercent;
    int afterCenterY = afterHeight * centerYPercent;
    
    afterX = sourceX - (afterCenterX - (_scaleCenterPoint.x() - sourceX));
    afterY = sourceY - (afterCenterY - (_scaleCenterPoint.y() - sourceY));
    
    if (afterX > _cropLeft) {
        afterX = _cropLeft;
    } else if (afterX + afterWidth < _cropRight) {
        afterX = _cropRight - afterWidth;
    }
    if (afterY > _cropTop) {
        afterY = _cropTop;
    } else if (afterY + afterHeight < _cropBottom) {
        afterY = _cropBottom - afterHeight;
    }
    
    _scaleResultImage = _sourceImage.scaledToWidth(afterWidth);
    _drawImageRect = QRectF(afterX, afterY, afterWidth, afterHeight);
    
    this->update();
}

void PhotoView::scaleEnded()
{
    _scaleCenterPoint = QPointF(0, 0);
    _scaleSourceRect = QRectF(_drawImageRect.x(), _drawImageRect.y(), _drawImageRect.width(), _drawImageRect.height());
}

void PhotoView::crop()
{
    if (_sourceImage.isNull()) {
        return;
    }
    
    QImage srcImage = _sourceImage;
    double scale = (double)_sourceImage.width() / (double)_scaleResultImage.width();
    
    int x = _cropLeft - _drawImageRect.x();
    int y = _cropTop - _drawImageRect.y();
    
    QImage resultImage = srcImage.copy(QRect(x * scale, y * scale, _cropWidth * scale, _cropHeight * scale));
    if (!resultImage.isNull()) {
        QString savePath = QString("%1/%2.png").arg(Utils::getImageSavePath()).arg(QDateTime::currentDateTime().toMSecsSinceEpoch());
        qDebug() << "crop result image save path:" << savePath;
        bool flag = resultImage.save(savePath, "png", 70);
        if (flag) {
            emit signal_cropSuccess(savePath);
        }
    }
    
}

void PhotoView::paint(QPainter *painter)
{
    if (!_cropAreaInited) {
        initCropArea();
    }
    
    QRectF mainRect = boundingRect();
    
    // ==== draw background
    painter->fillRect(mainRect, QBrush(Qt::black));

    // ==== draw image
    if (!_sourceImage.isNull()) {
        QImage drawImage = _scaleResultImage;
        painter->drawImage(_drawImageRect, drawImage);
        
        // ==== draw shadow
        QBrush shadowBursh(QColor(0, 0, 0, 128));
        QRectF topRect = QRectF(0, 0, mainRect.width(), _cropTop);
        QRectF leftRect = QRectF(0, _cropTop, _cropLeft, mainRect.height() - _cropTop);
        QRectF rightRect = QRectF(_cropRight, _cropTop, mainRect.width() - _cropRight, mainRect.height() - _cropTop);
        QRectF bottomRect = QRectF(_cropLeft, _cropBottom, _cropWidth, mainRect.height() - _cropBottom);
        
        painter->fillRect(topRect, shadowBursh);
        painter->fillRect(leftRect, shadowBursh);
        painter->fillRect(rightRect, shadowBursh);
        painter->fillRect(bottomRect, shadowBursh);
    }
    
    // ==== draw line
    painter->setPen(QPen(QColor("#ffffff"), 2));
    painter->drawLine(QPointF(_cropLeft, _cropTop), QPointF(_cropLeft, _cropBottom));    // left lin
    painter->drawLine(QPointF(_cropLeft, _cropTop), QPointF(_cropRight, _cropTop));      // top line
    painter->drawLine(QPointF(_cropRight, _cropTop), QPointF(_cropRight, _cropBottom));  // right line
    painter->drawLine(QPointF(_cropLeft, _cropBottom), QPointF(_cropRight, _cropBottom)); // bottom line
    
    int oneThirdWidth = _cropWidth / 3;
    int oneThirdHeight = _cropHeight / 3;
    painter->setPen(QPen(QColor("#80ffffff"), 2));
    painter->drawLine(QPointF(_cropLeft, _cropTop + oneThirdHeight), QPointF(_cropRight, _cropTop + oneThirdHeight));
    painter->drawLine(QPointF(_cropLeft, _cropTop + oneThirdHeight * 2), QPointF(_cropRight, _cropTop + oneThirdHeight * 2));
    painter->drawLine(QPointF(_cropLeft + oneThirdWidth, _cropTop), QPointF(_cropLeft + oneThirdWidth, _cropBottom));
    painter->drawLine(QPointF(_cropLeft + oneThirdWidth * 2, _cropTop), QPointF(_cropLeft + oneThirdWidth * 2, _cropBottom));
    
}

void PhotoView::slot_loadImageDataDone(QString path, QImage image)
{
    emit signal_loadingEnded();
    
    if (_path.compare(path) != 0) {
        return;
    }
    
    if (image.isNull()) {
        return;
    }
    
    _sourceImage = image;
    
    if (image.width() > image.height()) {
        _scaleResultImage = image.scaledToHeight(_cropHeight, Qt::SmoothTransformation);
    } else {
        _scaleResultImage = image.scaledToWidth(_cropWidth, Qt::SmoothTransformation);
    }
    
    int drawImageX = 0;
    int drawImageY = 0;
    int drawImageWidth = _scaleResultImage.width();
    int drawImageHeight = _scaleResultImage.height();
    
    if (drawImageWidth > _cropWidth) {
        drawImageX = _cropLeft - (drawImageWidth - _cropWidth) / 2;
    } else {
        drawImageX = _cropLeft;
    }
    if (drawImageHeight > _cropHeight) {
        drawImageY = _cropTop - (drawImageHeight - _cropHeight) / 2;
    } else {
        drawImageY = _cropTop;
    }
    
    _drawImageRect = QRectF(drawImageX, drawImageY, drawImageWidth, drawImageHeight);
    
    this->update();
}

double PhotoView::maxScale() const
{
    return _maxScale;
}

void PhotoView::setMaxScale(const double maxScale)
{
    if (_maxScale != maxScale) {
        _maxScale = maxScale;
        emit maxScaleChanged();
    }
}
