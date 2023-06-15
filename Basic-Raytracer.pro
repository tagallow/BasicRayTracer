#-------------------------------------------------
#
# Project created by QtCreator 2011-12-04T10:46:31
#
#-------------------------------------------------

QT       += core gui opengl xml

TARGET = Basic-Raytracer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Texture.cpp \
    Sphere.cpp \
    SemiCircle.cpp \
    Plane.cpp \
    objectglwidget.cpp \
    Matrix4.cpp \
    Material.cpp \
    Light.cpp \
    Cylinder.cpp \
    Cone.cpp

HEADERS  += mainwindow.h \
    Vector3D.h \
    Texture.h \
    Sphere.h \
    SemiCircle.h \
    Ray.h \
    Point3D.h \
    Plane.h \
    ObjectXMLReader.h \
    objectglwidget.h \
    Object.h \
    Matrix4.h \
    Material.h \
    Light.h \
    HitRecord.h \
    Cylinder.h \
    Cone.h \
    Box.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    input-1.txt \
    input.txt



