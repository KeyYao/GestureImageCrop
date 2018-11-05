#include "iosassetutils.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#import <Foundation/Foundation.h>
#import <Photos/Photos.h>
#import <UIKit/UIKit.h>

CGContextRef getCGContextFromImage(CGImageRef image);
QImage convertUIImageToQImage(UIImage *image);

IOSAssetUtils::IOSAssetUtils()
{
    
}

QString IOSAssetUtils::getDocumentPath()
{
    NSString *pathString = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES).firstObject;
    return QString::fromUtf8([pathString UTF8String]);
}

QString IOSAssetUtils::getAssetCollectionList()
{
    __block BOOL authorized = NO;
    if (PHPhotoLibrary.authorizationStatus != PHAuthorizationStatusAuthorized) {
        dispatch_semaphore_t seamphore = dispatch_semaphore_create(0);
        [PHPhotoLibrary requestAuthorization:^(PHAuthorizationStatus status) {
            if (status == PHAuthorizationStatusAuthorized) {
                authorized = YES;
            }
            dispatch_semaphore_signal(seamphore);
        }];
        dispatch_semaphore_wait(seamphore, DISPATCH_TIME_FOREVER);
    } else {
        authorized = YES;
    }
    
    QString json = "";
    
    if (authorized) {
        QJsonArray array;
        NSMutableArray *collectionArray = [NSMutableArray array];
        
        PHFetchOptions *assetOptions = [[PHFetchOptions alloc] init];
        assetOptions.predicate = [NSPredicate predicateWithFormat:@"mediaType == %d", PHAssetMediaTypeImage];
        
        PHAssetCollection *defaultCollection = [PHAssetCollection fetchAssetCollectionsWithType:PHAssetCollectionTypeSmartAlbum subtype:PHAssetCollectionSubtypeSmartAlbumUserLibrary options:nil][0];
        [collectionArray addObject: defaultCollection];
        
        PHFetchResult<PHCollection *> *userCollection = [PHCollectionList fetchTopLevelUserCollectionsWithOptions:nil];
        for (PHCollection *c in userCollection) {
            if ([c isKindOfClass:[PHAssetCollection classForCoder]]) {
                [collectionArray addObject:(PHAssetCollection*)c];
            }
        }
        
        for (PHAssetCollection *c in collectionArray) {
            PHFetchResult *result = [PHAsset fetchAssetsInAssetCollection:c options:assetOptions];
            QString url = "";
            if (result.count > 0) {
                PHAsset *asset = result[0];
                url = QString::fromUtf8([asset.localIdentifier UTF8String]);
            } else {
                continue;
            }
            
            QJsonObject itemObject;
            itemObject.insert("identifier", QString::fromUtf8([c.localIdentifier UTF8String]));
            itemObject.insert("name", QString::fromUtf8([c.localizedTitle UTF8String]));
            itemObject.insert("url", url);
            array.append(itemObject);
        }
        
        QJsonDocument doc;
        doc.setArray(array);
        json = QString(doc.toJson(QJsonDocument::Compact));
        
    } else {
        return QString();
    }
    
    return json;
}

QString IOSAssetUtils::getAssetList(QString collectionIdentifier)
{
    __block BOOL authorized = NO;
    if (PHPhotoLibrary.authorizationStatus != PHAuthorizationStatusAuthorized) {
        dispatch_semaphore_t seamphore = dispatch_semaphore_create(0);
        [PHPhotoLibrary requestAuthorization:^(PHAuthorizationStatus status) {
            if (status == PHAuthorizationStatusAuthorized) {
                authorized = YES;
            }
            dispatch_semaphore_signal(seamphore);
        }];
        dispatch_semaphore_wait(seamphore, DISPATCH_TIME_FOREVER);
    } else {
        authorized = YES;
    }
    
    QString json = "";
    if (authorized) {
        QJsonArray array;
        
        PHFetchResult<PHAssetCollection *> *collectionList = [PHAssetCollection fetchAssetCollectionsWithLocalIdentifiers:@[collectionIdentifier.toNSString()] options:nil];
        if (collectionList.count <= 0) {
            return json;
        }
        
        PHAssetCollection *collection = collectionList[0];
        PHFetchOptions *assetOptions = [[PHFetchOptions alloc] init];
        assetOptions.predicate = [NSPredicate predicateWithFormat:@"mediaType == %d", PHAssetMediaTypeImage];
        assetOptions.sortDescriptors = @[[NSSortDescriptor sortDescriptorWithKey:@"creationDate" ascending:NO]];
        PHFetchResult *result = [PHAsset fetchAssetsInAssetCollection:collection options:assetOptions];
        for (PHAsset *asset in result) {
            QJsonObject itemObject;
            itemObject.insert("name", QString::fromUtf8([[asset valueForKey:@"filename"] UTF8String]));
            itemObject.insert("path", QString::fromUtf8([asset.localIdentifier UTF8String]));
            array.append(itemObject);
        }
        
        QJsonDocument doc;
        doc.setArray(array);
        json = QString(doc.toJson(QJsonDocument::Compact));
        
    } else {
        return QString();
    }
    
    return json;
}

QImage IOSAssetUtils::getThumbnail(QString assetIdentifier)
{
    NSString *identifierString = assetIdentifier.toNSString();
    PHFetchResult<PHAsset *> *result = [PHAsset fetchAssetsWithLocalIdentifiers:@[identifierString] options:nil];
    if (!result || result.count == 0) {
        return QImage();
    }
    PHAsset *asset = result.firstObject;
    
    if (!asset) {
        return QImage();
    }
    
    PHImageRequestOptions *options = [[PHImageRequestOptions alloc] init];
    options.synchronous = YES;
    options.deliveryMode = PHImageRequestOptionsDeliveryModeOpportunistic;
    options.resizeMode = PHImageRequestOptionsResizeModeExact;
    options.networkAccessAllowed = YES;
    
    __block dispatch_semaphore_t seamphore = dispatch_semaphore_create(0);
    __block QImage image;
    
    [[PHCachingImageManager defaultManager] requestImageForAsset:asset targetSize:CGSizeMake(128, 128) contentMode:PHImageContentModeAspectFill options:options resultHandler:^(UIImage * _Nullable result, NSDictionary * _Nullable info) {
        if (result) {
            NSData *imageData = UIImagePNGRepresentation(result);
            if (imageData.length > 0) {
                image = QImage::fromData(static_cast<const uchar *>(imageData.bytes), imageData.length);
            } else {
                image = convertUIImageToQImage(result);
            }
        }
        dispatch_semaphore_signal(seamphore);
    }];
    dispatch_semaphore_wait(seamphore, DISPATCH_TIME_FOREVER);
    
    return image;
}

QImage IOSAssetUtils::getImage(QString assetIdentifier)
{
    NSString *identifierString = assetIdentifier.toNSString();
    PHFetchResult<PHAsset *> *result = [PHAsset fetchAssetsWithLocalIdentifiers:@[identifierString] options:nil];
    if (!result || result.count == 0) {
        return QImage();
    }
    PHAsset *asset = result.firstObject;
    
    if (!asset) {
        return QImage();
    }
    
    __block QImage image;
    __block dispatch_semaphore_t seamphore = dispatch_semaphore_create(0);
    
    PHImageRequestOptions *options = [[PHImageRequestOptions alloc] init];
    options.synchronous = YES;
    options.deliveryMode = PHImageRequestOptionsDeliveryModeHighQualityFormat;
    options.resizeMode = PHImageRequestOptionsResizeModeExact;
    options.networkAccessAllowed = YES;
    [[PHImageManager defaultManager] requestImageDataForAsset:asset options:options resultHandler:^(NSData * _Nullable imageData, NSString * _Nullable dataUTI, UIImageOrientation orientation, NSDictionary * _Nullable info) {
        if (imageData) {
            if (imageData.length > 0) {
                image = QImage::fromData(static_cast<const uchar *>(imageData.bytes), imageData.length);
            }
        }
        dispatch_semaphore_signal(seamphore);
    }];
    dispatch_semaphore_wait(seamphore, DISPATCH_TIME_FOREVER);
    
    return image;
}


CGContextRef getCGContextFromImage(CGImageRef image) {
    CGContextRef context = NULL;
    CGColorSpaceRef colorSpace;
    uint32_t *bitmapData;

    size_t bitsPerPixel = 32;
    size_t bitsPerComponent = 8;
    size_t bytesPerPixel = bitsPerPixel / bitsPerComponent;

    size_t width = CGImageGetWidth(image);
    size_t height = CGImageGetHeight(image);

    size_t bytesPerRow = width * bytesPerPixel;
    size_t bufferLength = bytesPerRow * height;

    colorSpace = CGColorSpaceCreateDeviceRGB();

    if(!colorSpace) {
        qWarning() << "Error allocating color space RGB";
        return NULL;
    }

    // Allocate memory for image data
    bitmapData = (uint32_t *)malloc(bufferLength);

    if(!bitmapData) {
        qWarning() << "Error allocating memory for bitmap";
        CGColorSpaceRelease(colorSpace);
        return NULL;
    }

    //Create bitmap context

    context = CGBitmapContextCreate(bitmapData,
                                    width,
                                    height,
                                    bitsPerComponent,
                                    bytesPerRow,
                                    colorSpace,
                                    kCGImageAlphaPremultipliedLast);    // RGBA
    if(!context) {
        free(bitmapData);
        qWarning() << "Bitmap context not created";
    }

    CGColorSpaceRelease(colorSpace);

    return context;
}

QImage convertUIImageToQImage(UIImage *image)
{
    CGImageRef imageRef = image.CGImage;

    // Create a bitmap context to draw the uiimage into
    CGContextRef context = getCGContextFromImage(imageRef);

    if(!context) {
        return QImage();
    }

    size_t width = CGImageGetWidth(imageRef);
    size_t height = CGImageGetHeight(imageRef);

    CGRect rect = CGRectMake(0, 0, width, height);

    // Draw image into the context to get the raw image data
    CGContextDrawImage(context, rect, imageRef);

    // Get a pointer to the data
    unsigned char *bitmapData = (unsigned char*)CGBitmapContextGetData(context);

    // Copy the data and release the memory (return memory allocated with new)
    size_t bytesPerRow = CGBitmapContextGetBytesPerRow(context);

    if(bitmapData) {

    } else {
        qWarning() << "Error getting bitmap pixel data";
    }

    int w = bytesPerRow / 4;
    int h = height;
    QImage img = QImage(w, h, QImage::Format_RGBA8888_Premultiplied);
    img.fill(Qt::black);
    memcpy(img.bits(), bitmapData, w * h * 4);

    free(bitmapData);
    CGContextRelease(context);

    return img;
}
