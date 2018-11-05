#include "loadimagedatathread.h"

#include <QDebug>

#ifdef Q_OS_IOS
#include "../utils/iosassetutils.h"
#endif

LoadImageDataThread::LoadImageDataThread(QString path, QObject *parent) : QThread(parent)
{
    this->_path = path;
}

void LoadImageDataThread::run()
{
    QImage image;
    bool result = false;
#ifdef Q_OS_ANDROID
    result = image.load(_path);
#endif
#ifdef Q_OS_IOS
    IOSAssetUtils utils;
    image = utils.getImage(_path);
    result = !image.isNull();
#endif
    qWarning() << "LoadImageDataThread load result:" << result;
    
    emit signal_done(this->_path, image);
}
