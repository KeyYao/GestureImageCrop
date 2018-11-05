#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "widget/photoview.h"
#include "widget/imagethumbnailview.h"
#include "service/photoservice.h"

int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    
    QGuiApplication app(argc, argv);
    
    QQmlApplicationEngine engine;
    
    qmlRegisterType<PhotoView>("PhotoView", 1, 0, "PhotoView");
    qmlRegisterType<ImageThumbnailView>("ImageThumbnailView", 1, 0, "ImageThumbnailView");
    qmlRegisterType<PhotoService>("PhotoService", 1, 0, "PhotoService");
    
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    
    if (engine.rootObjects().isEmpty())
        return -1;
    
    return app.exec();
}
