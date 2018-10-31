#ifndef ALBUMENTRY_H
#define ALBUMENTRY_H

#include <QObject>

class AlbumEntry : public QObject
{
    Q_OBJECT
    
    Q_PROPERTY(QString identifier READ identifier WRITE setIdentifier NOTIFY identifierChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged)
    
public:
    explicit AlbumEntry(QObject *parent = nullptr);
    
    QString identifier() const;
    void setIdentifier(const QString &identifier);
    
    QString name() const;
    void setName(const QString &name);
    
    QString url() const;
    void setUrl(const QString &url);
    
private:
    QString _identifier = "";
    QString _name = "";
    QString _url = "";
    
signals:
    void identifierChanged();
    void nameChanged();
    void urlChanged();
    
public slots:
};

#endif // ALBUMENTRY_H
