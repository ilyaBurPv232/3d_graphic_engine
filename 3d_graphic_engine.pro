QT       += core gui openglwidgets opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cameracontroller.cpp \
    collisionresolution.cpp \
    main.cpp \
    mainwindow.cpp \
    openglwidget.cpp \
    physicalprimitives.cpp \
    primitives.cpp \
    scene.cpp \
    shadermanager.cpp \
    shape.cpp \
    skybox.cpp \
    texturemanager.cpp \
    rigidbody.cpp \
    collisiondetector.cpp \
    physicalobject.cpp \
    physicalworld.cpp \
    physicmath.cpp \

HEADERS += \
    cameracontroller.h \
    collisionresolution.h \
    mainwindow.h \
    openglwidget.h \
    physicalprimitives.h \
    primitives.h \
    scene.h \
    shadermanager.h \
    shape.h \
    skybox.h \
    texturemanager.h \
    rigidbody.h \
    collisiondetector.h \
    physicalobject.h \
    physicalworld.h \
    physicmath.h

FORMS += \
    mainwindow.ui

LIBS += -lopengl32

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    fshader.fsh \
    vshader.vsh

RESOURCES += \
    shaders.qrc \
    skyboxes.qrc \
    textures.qrc
