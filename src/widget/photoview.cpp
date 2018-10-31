#include "photoview.h"

#include <QPainter>
#include <QImage>
#include <QDebug>
#include <QDateTime>

#include "../utils/utils.h"

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
    QImage image;
    bool result = image.load(path);
    qWarning() << "image load result:" << result;
    
    if (!result) {
        return;
    }
    
    if (image.width() > image.height()) {
        _sourceImage = image.scaledToHeight(_cropHeight, Qt::SmoothTransformation);
    } else {
        _sourceImage = image.scaledToWidth(_cropWidth, Qt::SmoothTransformation);
    }
    
    int drawImageX = 0;
    int drawImageY = 0;
    int drawImageWidth = _sourceImage.width();
    int drawImageHeight = _sourceImage.height();
    
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
    
    if (afterWidth > _sourceImage.width() * _maxScale) {
        afterWidth = _sourceImage.width() * _maxScale;
        afterHeight = _sourceImage.height() * _maxScale;
    } else if (afterWidth < _sourceImage.width()) {
        afterWidth = _sourceImage.width();
        afterHeight = _sourceImage.height();
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
    QImage srcImage;
    if (_scaleResultImage.isNull()) {
        srcImage = _sourceImage;
    } else {
        srcImage = _scaleResultImage;
    }
    
    int x = _cropLeft - _drawImageRect.x();
    int y = _cropTop - _drawImageRect.y();
    
    QImage resultImage = srcImage.copy(QRect(x, y, _cropWidth, _cropHeight));
    if (!resultImage.isNull()) {
        QString savePath = QString("%1/%2.png").arg(Utils::getImageSavePath()).arg(QDateTime::currentDateTime().toMSecsSinceEpoch());
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
    if (_sourceImage.isNull()) {
        return;
    }

    QImage drawImage;
    if (_scaleResultImage.isNull()) {
        drawImage = _sourceImage;
    } else {
        drawImage = _scaleResultImage;
    }
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
