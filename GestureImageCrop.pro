QT += quick
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += src/main.cpp \
    src/widget/photoview.cpp \
    src/entry/albumentry.cpp \
    src/service/photoservice.cpp \
    src/entry/fileentry.cpp \
    src/utils/utils.cpp \
    src/task/loadimagedatathread.cpp

HEADERS += \
    src/widget/photoview.h \
    src/entry/albumentry.h \
    src/service/photoservice.h \
    src/entry/fileentry.h \
    src/utils/utils.h \
    src/task/loadimagedatathread.h

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

android {
    QT += androidextras

    DISTFILES += \
        android/AndroidManifest.xml \
        android/build.gradle \
        android/gradle/wrapper/gradle-wrapper.jar \
        android/gradle/wrapper/gradle-wrapper.properties \
        android/gradlew \
        android/res/values/libs.xml \
        android/gradlew.bat \

    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
}

ios {
    LIBS += -framework UIKit

    QMAKE_LFLAGS    += -framework OpenGLES
    QMAKE_LFLAGS    += -framework GLKit
    QMAKE_LFLAGS    += -framework QuartzCore
    QMAKE_LFLAGS    += -framework UIKit
    QMAKE_LFLAGS    += -framework Photos
    QMAKE_LFLAGS    += -framework PhotosUI
    QMAKE_LFLAGS    += -framework CoreGraphics
}

DISTFILES += \
    android/src/moe/key/yao/gestureimagecrop/GestureImageCropApplication.java \
    android/src/moe/key/yao/gestureimagecrop/utils/MediaUtils.java \
    android/src/moe/key/yao/gestureimagecrop/bean/AlbumBean.java \
    android/src/moe/key/yao/gestureimagecrop/bean/FileBean.java
