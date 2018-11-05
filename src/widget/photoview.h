#ifndef PHOTOVIEW_H
#define PHOTOVIEW_H

#include <QQuickPaintedItem>
#include <QImage>
#include <QPointF>
#include <QRectF>

class PhotoView : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(double maxScale READ maxScale WRITE setMaxScale NOTIFY maxScaleChanged)
    
public:
    PhotoView(QQuickItem *parent = nullptr);
    
    Q_INVOKABLE void setSrc(QString path);
    
    Q_INVOKABLE void moveBegan();
    Q_INVOKABLE void moveUpdate(int offsetX, int offsetY);
    Q_INVOKABLE void moveEnded();
    
    Q_INVOKABLE void scaleBegan(int centerX, int centerY);
    Q_INVOKABLE void scaleUpdate(float scale);
    Q_INVOKABLE void scaleEnded();
    
    Q_INVOKABLE void crop();
    
    void paint(QPainter *painter);
    
    double maxScale() const;
    void setMaxScale(const double maxScale);
    
private:
    double _maxScale;
    
    QString _path;
    
    QImage _sourceImage;
    QRectF _drawImageRect;
    
    int _cropLeft;
    int _cropTop;
    int _cropRight;
    int _cropBottom;
    int _cropWidth;
    int _cropHeight;
    
    QPointF _moveTouchPoint;
    
    QPointF _scaleCenterPoint;
    QRectF _scaleSourceRect;
    QImage _scaleResultImage;
    
    bool _cropAreaInited = false;
    void initCropArea();
    
signals:
    void signal_loadingStarted();
    void signal_loadingEnded();
    
    void signal_cropAreaInited();
    void signal_cropSuccess(QString path);
    
    void maxScaleChanged();
    
public slots:
    void slot_loadImageDataDone(QString path, QImage image);
};

#endif // PHOTOVIEW_H
