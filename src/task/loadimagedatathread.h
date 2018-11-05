#ifndef LOADIMAGEDATATHREAD_H
#define LOADIMAGEDATATHREAD_H

#include <QThread>
#include <QImage>

class LoadImageDataThread : public QThread
{
    Q_OBJECT
public:
    explicit LoadImageDataThread(QString path, QObject *parent = nullptr);
    
    void run();
    
private:
    QString _path;
    
signals:
    void signal_done(QString path, QImage image);
    
public slots:
};

#endif // LOADIMAGEDATATHREAD_H
