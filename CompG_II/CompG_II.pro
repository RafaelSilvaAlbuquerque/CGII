#-------------------------------------------------
#
# Project created by QtCreator 2015-02-21T16:48:57
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CompG_II
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    objeto.cpp \
    display.cpp \
    algebra.cpp \
    iluminacao.cpp \
    camera.cpp \
    vect.cpp \
    transformation.cpp \
    matrix4x4.cpp \
    boundbox.cpp

HEADERS  += mainwindow.h \
    objeto.h \
    display.h \
    algebra.h \
    iluminacao.h \
    camera.h \
    vect.h \
    transformation.h \
    matrix4x4.h \
    boundbox.h

FORMS    += mainwindow.ui

LIBS     += -lGLU -lglut -fopenmp

QMAKE_CXXFLAGS += -fopenmp
QMAKE_LFLAGS += -fopenmp

QT += xml #necessario colocar  para o xml funcionar

