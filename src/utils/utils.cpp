#include "utils.h"

#include <QStandardPaths>

Utils::Utils(QObject *parent) : QObject(parent)
{
    
}

QString Utils::getImageSavePath()
{
    QStringList pathList = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
    QString path;
    foreach (QString p, pathList) {
        if (p.compare("/") != 0 && p.compare("") != 0) {
            path = p;
            break;
        }
    }
    return path;
}
