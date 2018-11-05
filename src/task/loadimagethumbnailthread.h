#ifndef LOADIMAGETHUMBNAILTHREAD_H
#define LOADIMAGETHUMBNAILTHREAD_H

#include <QObject>
#include <QRunnable>
#include <QSizeF>
#include <QImage>

class LoadImageThumbnailThread : public QObject, public QRunnable
{
    Q_OBJECT
public:
    LoadImageThumbnailThread(QString source, QSizeF targetSize, QObject *parent = nullptr);
    
    void run();
    
private:
    QString _source;
    QSizeF _targetSize;
    
signals:
    void signal_done(QString source, QImage image);
    
public slots:
};

#endif // LOADIMAGETHUMBNAILTHREAD_H
