#ifndef IMAGETHUMBNAILVIEW_H
#define IMAGETHUMBNAILVIEW_H

#include <QQuickPaintedItem>
#include <QImage>

class ImageThumbnailView : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
    
public:
    explicit ImageThumbnailView(QQuickItem *parent = nullptr);
    
    void paint(QPainter *painter);
    
    QString source() const;
    void setSource(const QString &source);
    
private:
    QString _source;
    QImage _image;
    bool _viewInited = false;
    QRectF _mainRect;
    
    void loadThumbnail();
    
signals:
    void signal_loadingStarted();
    void signal_loadingEnded();
    
    void sourceChanged();
    
public slots:
    void slot_sourceChanged();
    void slot_loadDone(QString source, QImage image);
};

#endif // IMAGETHUMBNAILVIEW_H
