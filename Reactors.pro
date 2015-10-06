#-------------------------------------------------
#
# Project created by QtCreator 2015-03-23T23:00:50
#
#-------------------------------------------------

QT       += core gui multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = Reactors
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    schemapipeline.cpp \
    schemaflowmeter.cpp \
    schemaitem.cpp \
    schemavalve.cpp \
    schemastream.cpp \
    schemaport.cpp \
    schemaview.cpp \
    schemavessel.cpp \
    pfdcontrol.cpp \
    qcustomplot/qcustomplot.cpp \
    schemacstr.cpp \
    libxls/src/ole.c \
    libxls/src/xls.c \
    libxls/src/xlstool.c \
    modelcell.cpp \
    schemadata.cpp

win32: SOURCES += libxls/src/localcharset.c

HEADERS  += mainwindow.h \
    schemapipeline.h \
    schemaflowmeter.h \
    schemaitem.h \
    schemavalve.h \
    schemastream.h \
    schemaport.h \
    schemaview.h \
    schemavessel.h \
    pfdcontrol.h \
    qcustomplot/qcustomplot.h \
    schemacstr.h \
    libxls/include/libxls/brdb.c.h \
    libxls/include/libxls/brdb.h \
    libxls/include/libxls/ole.h \
    libxls/include/libxls/xls.h \
    libxls/include/libxls/xlsstruct.h \
    libxls/include/libxls/xlstool.h \
    libxls/include/libxls/xlstypes.h \
    libxls/include/libxls/config.h \
    modelcell.h \
    schemadata.h

win32: HEADERS += libxls/include/libxls/localcharset.h

FORMS    += mainwindow.ui

TRANSLATIONS    = Reactors_ru.ts \
                  Reactors_de.ts

INCLUDEPATH += $$PWD/libxls/include
DEPENDPATH += $$PWD/libxls/include

RESOURCES += \
    reactors.qrc

RC_FILE = icon.rc

DISTFILES += \
    icon.rc

win32: LIBS += -L$$PWD/gsl-1.8-lib/lib/ -llibgsl


win32:INCLUDEPATH += $$PWD/gsl-1.8-lib/include
win32:DEPENDPATH += $$PWD/gsl-1.8-lib/include

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += gsl

win32: LIBS += -L$$PWD/gsl-1.8-lib/lib/ -lgslcblas

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/gsl-1.8-lib/lib/gslcblas.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/gsl-1.8-lib/lib/libgslcblas.a
