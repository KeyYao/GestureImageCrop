#ifndef IOSASSETUTILS_H
#define IOSASSETUTILS_H

#include <QString>
#include <QImage>

class IOSAssetUtils
{
public:
    IOSAssetUtils();
    
    QString getDocumentPath();
    
    QString getAssetCollectionList();
    
    QString getAssetList(QString collectionIdentifier);
    
    QImage getThumbnail(QString assetIdentifier);
    
    QImage getImage(QString assetIdentifier);
    
};

#endif // IOSASSETUTILS_H
