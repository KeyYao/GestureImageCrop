#ifndef PHOTOSERVICE_H
#define PHOTOSERVICE_H

#include <QObject>

class PhotoService : public QObject
{
    Q_OBJECT
public:
    explicit PhotoService(QObject *parent = nullptr);
    
    Q_INVOKABLE QList<QObject*> loadAlbumList();
    Q_INVOKABLE QList<QObject*> loadFileList(QString identifier);
    
signals:
    
public slots:
};

#endif // PHOTOSERVICE_H
