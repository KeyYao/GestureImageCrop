#include "albumentry.h"

AlbumEntry::AlbumEntry(QObject *parent) : QObject(parent)
{
    
}

QString AlbumEntry::identifier() const
{
    return _identifier;
}

void AlbumEntry::setIdentifier(const QString &identifier)
{
    if (_identifier != identifier) {
        _identifier = identifier;
        emit identifierChanged();
    }
}

QString AlbumEntry::name() const
{
    return _name;
}

void AlbumEntry::setName(const QString &name)
{
    if (_name != name) {
        _name = name;
        emit nameChanged();
    }
}

QString AlbumEntry::url() const
{
    return _url;
}

void AlbumEntry::setUrl(const QString &url)
{
    if (_url != url) {
        _url = url;
        emit urlChanged();
    }
}
