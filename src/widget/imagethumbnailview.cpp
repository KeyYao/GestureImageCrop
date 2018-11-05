#include "imagethumbnailview.h"

#include <QPainter>
#include <QImage>
#include <QDebug>

#include "../task/imagethumbnailloader.h"
#include "../task/loadimagethumbnailthread.h"

ImageThumbnailView::ImageThumbnailView(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    connect(this, SIGNAL(sourceChanged()), this, SLOT(slot_sourceChanged()));
    this->setClip(true);
}

QString ImageThumbnailView::source() const
{
    return _source;
}

void ImageThumbnailView::setSource(const QString &source)
{
    if (_source.isNull()) {
        _source = source;
        emit sourceChanged();
        return;
    }
    if (_source.compare(source) != 0) {
        _source = source;
        emit sourceChanged();
    }
}

void ImageThumbnailView::paint(QPainter *painter)
{
    if (!_viewInited) {
        _viewInited = true;
        _mainRect = boundingRect();
        loadThumbnail();
        return;
    }
    
    if (_image.isNull()) {
        return;
    }
    
    painter->drawImage(_mainRect, _image);
}

void ImageThumbnailView::loadThumbnail()
{
    if (_source.isNull() || _source.isEmpty() || _mainRect.width() == 0 || _mainRect.height() == 0) {
        return;
    }
    
    
    QImage cacheImge = ImageThumbnailLoader::instance()->getCache(_source, _mainRect.size());
    if (!cacheImge.isNull()) {
        _image = cacheImge;
        this->update();
        return;
    }
    
    LoadImageThumbnailThread *thread = new LoadImageThumbnailThread(_source, _mainRect.size());
    connect(thread, SIGNAL(signal_done(QString,QImage)), this, SLOT(slot_loadDone(QString,QImage)));
    ImageThumbnailLoader::instance()->startTask(thread);
}

void ImageThumbnailView::slot_sourceChanged()
{
    loadThumbnail();
}

void ImageThumbnailView::slot_loadDone(QString source, QImage image)
{
    if (_source != source) {
        return;
    }
    this->_image = image;
    
    this->update();
}
