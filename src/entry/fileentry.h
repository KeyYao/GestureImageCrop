#ifndef FILEENTRY_H
#define FILEENTRY_H

#include <QObject>

class FileEntry : public QObject
{
    Q_OBJECT
    
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)
    
public:
    explicit FileEntry(QObject *parent = nullptr);
    
    QString name() const;
    void setName(const QString &name);
    
    QString path() const;
    void setPath(const QString &path);
    
private:
    QString _name;
    QString _path;
    
signals:
    void nameChanged();
    void pathChanged();
    
public slots:
};

#endif // FILEENTRY_H
