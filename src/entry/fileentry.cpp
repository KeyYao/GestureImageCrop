#include "fileentry.h"

FileEntry::FileEntry(QObject *parent) : QObject(parent)
{
    
}

QString FileEntry::name() const
{
    return _name;
}

void FileEntry::setName(const QString &name)
{
    if (_name != name) {
        _name = name;
        emit nameChanged();
    }
}

QString FileEntry::path() const
{
    return _path;
}

void FileEntry::setPath(const QString &path)
{
    if (_path != path) {
        _path = path;
        emit pathChanged();
    }
}
