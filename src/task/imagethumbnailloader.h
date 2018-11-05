#ifndef IMAGETHUMBNAILLOADER_H
#define IMAGETHUMBNAILLOADER_H

#include <QObject>
#include <QImage>
#include <QSizeF>
#include <QCache>
#include <QRunnable>
#include <QThreadPool>

class ImageThumbnailLoader : public QObject
{
    Q_OBJECT
public:
    ~ImageThumbnailLoader();
    
    static ImageThumbnailLoader* instance();
    
    void startTask(QRunnable *task);
    
    void saveCache(QString key, QImage image);
    
    QImage getCache(QString key, QSizeF targetSize = QSizeF(0, 0));
    
private:
    ImageThumbnailLoader();
    
    QThreadPool *_threadPool = nullptr;
    QCache<QString, QImage> _memoryCache;
    
signals:
    
public slots:
};

#endif // IMAGETHUMBNAILLOADER_H
