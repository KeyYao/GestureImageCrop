#include "photoservice.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include "../entry/albumentry.h"
#include "../entry/fileentry.h"

#ifdef Q_OS_ANDROID
#include <QAndroidJniEnvironment>
#include <QAndroidJniObject>
#include <jni.h>
#endif
#ifdef Q_OS_IOS
#include "../utils/iosassetutils.h"
#endif

PhotoService::PhotoService(QObject *parent) : QObject(parent)
{
    
}

QList<QObject*> PhotoService::loadAlbumList()
{
    QString json = "";
#ifdef Q_OS_ANDROID
    QAndroidJniObject albumList = QAndroidJniObject::callStaticObjectMethod("moe/key/yao/gestureimagecrop/utils/MediaUtils", "getAlbumList", "()Ljava/lang/String;");
    json = albumList.toString();
#endif
#ifdef Q_OS_IOS
    IOSAssetUtils utils;
    json = utils.getAssetCollectionList();
#endif
    
    QList<QObject*> data;
    
    if (!json.isEmpty()) {
        QByteArray jsonData = json.toUtf8();
        QJsonDocument doc = QJsonDocument::fromJson(jsonData);
        if (!doc.isNull()) {
            QJsonArray array = doc.array();
            foreach (QJsonValue itemValue, array) {
                QJsonObject itemObject = itemValue.toObject();
                AlbumEntry *entry = new AlbumEntry(this);
                entry->setIdentifier(itemObject.value("identifier").toString());
                entry->setName(itemObject.value("name").toString());
                entry->setUrl(itemObject.value("url").toString());
                data.append(entry);
            }
        }
    }
    
    return data;
}

QList<QObject*> PhotoService::loadFileList(QString identifier)
{
    QString json = "";
#ifdef Q_OS_ANDROID
    QAndroidJniObject identifierObj = QAndroidJniObject::fromString(identifier);
    QAndroidJniObject fileList = QAndroidJniObject::callStaticObjectMethod("moe/key/yao/gestureimagecrop/utils/MediaUtils", "getPhotoFileList", "(Ljava/lang/String;)Ljava/lang/String;", identifierObj.object<jstring>());
    json = fileList.toString();
#endif
#ifdef Q_OS_IOS
    IOSAssetUtils utils;
    json = utils.getAssetList(identifier);
#endif
    
    QList<QObject*> data;
    
    if (!json.isEmpty()) {
        QByteArray jsonData = json.toUtf8();
        QJsonDocument doc = QJsonDocument::fromJson(jsonData);
        if (!doc.isNull()) {
            QJsonArray array = doc.array();
            foreach (QJsonValue itemValue, array) {
                QJsonObject itemObject = itemValue.toObject();
                FileEntry *entry = new FileEntry(this);
                entry->setName(itemObject.value("name").toString());
                entry->setPath(itemObject.value("path").toString());
                data.append(entry);
            }
        }
    }
    
    return data;
}
