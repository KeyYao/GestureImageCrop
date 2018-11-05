#include "utils.h"

#ifdef Q_OS_ANDROID
#include <QStandardPaths>
#endif
#ifdef Q_OS_IOS
#include "iosassetutils.h"
#endif

Utils::Utils(QObject *parent) : QObject(parent)
{
    
}

QString Utils::getImageSavePath()
{
#ifdef Q_OS_ANDROID
    QStringList pathList = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
    QString path;
    foreach (QString p, pathList) {
        if (p.compare("/") != 0 && p.compare("") != 0) {
            path = p;
            break;
        }
    }
    return path;
#endif
#ifdef Q_OS_IOS
    IOSAssetUtils utils;
    return utils.getDocumentPath();
#endif
    return "";
}
