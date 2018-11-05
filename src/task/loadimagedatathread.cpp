#include "loadimagedatathread.h"

#include <QDebug>

LoadImageDataThread::LoadImageDataThread(QString path, QObject *parent) : QThread(parent)
{
    this->_path = path;
}

void LoadImageDataThread::run()
{
    QImage image;
    bool result = image.load(_path);
    qWarning() << "LoadImageDataThread load result:" << result;
    
    emit signal_done(this->_path, image);
}
