#-------------------------------------------------
#
# Project created by QtCreator 2015-03-23T23:00:50
#
#-------------------------------------------------

QT       += core gui multimediawidgets xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport sql

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
    pfdcontrol.cpp \
    qcustomplot/qcustomplot.cpp \
    libxls/src/ole.c \
    libxls/src/xls.c \
    libxls/src/xlstool.c \
    modelcell.cpp \
    schemadata.cpp \
    schemadb.cpp \
    modelcstr.cpp \
    schemacstr.cpp \
    schemalogger.cpp \
    schemacell.cpp \
    schemaconfig.cpp \
    modelitem.cpp \
    modelvalve.cpp \
    modelflowmeter.cpp \
    qwt-mml/qwt_mml_document.cpp \
    qwt-mml/qwt_mml_entity_table.cpp \
    formulaview.cpp \
    schemaevent.cpp \
    schemascene.cpp \
    schemapfr.cpp \
    diffusemodel.cpp

win32: SOURCES += libxls/src/localcharset.c

HEADERS  += mainwindow.h \
    schemapipeline.h \
    schemaflowmeter.h \
    schemaitem.h \
    schemavalve.h \
    schemastream.h \
    schemaport.h \
    schemaview.h \
    pfdcontrol.h \
    qcustomplot/qcustomplot.h \
    libxls/include/libxls/brdb.c.h \
    libxls/include/libxls/brdb.h \
    libxls/include/libxls/ole.h \
    libxls/include/libxls/xls.h \
    libxls/include/libxls/xlsstruct.h \
    libxls/include/libxls/xlstool.h \
    libxls/include/libxls/xlstypes.h \
    libxls/include/libxls/config.h \
    modelcell.h \
    schemadata.h \
    schemadb.h \
    modelcstr.h \
    schemacstr.h \
    schemalogger.h \
    schemacell.h \
    schemaconfig.h \
    modelitem.h \
    modelvalve.h \
    modelflowmeter.h \
    qwt-mml/qwt_mml_document.h \
    qwt-mml/qwt_mml_entity_table.h \
    formulaview.h \
    schemaevent.h \
    schemascene.h \
    schemapfr.h \
    diffusemodel.h

win32: HEADERS += libxls/include/libxls/localcharset.h

FORMS    += mainwindow.ui

TRANSLATIONS    = Reactors_ru_RU.ts \
                  Reactors_de_DE.ts

INCLUDEPATH += $$PWD/libxls/include
DEPENDPATH += $$PWD/libxls/include

INCLUDEPATH += $$PWD/libssh-0.7.2/include
DEPENDPATH += $$PWD/libssh-0.7.2/include


RESOURCES += \
    reactors.qrc

RC_FILE = icon.rc

DISTFILES += \
    icon.rc

CONFIG += c++14

win32: LIBS += -L$$PWD/gsl-1.15-lib/lib/ -llibgsl


win32:INCLUDEPATH += $$PWD/gsl-1.15-lib/include
win32:DEPENDPATH += $$PWD/gsl-1.15-lib/include

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += gsl

win32: LIBS += -L$$PWD/gsl-1.15-lib/lib/ -lgslcblas

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/gsl-1.15-lib/lib/gslcblas.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/gsl-1.15-lib/lib/libgslcblas.a

INCLUDEPATH += $$PWD/qwt-mml/
DEPENDPATH += $$PWD/qwt-mml/

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/qwt-6.1.2/lib/ -lqwt
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/qwt-6.1.2/lib/ -lqwtd

#INCLUDEPATH += $$PWD/qwt-6.1.2/src/
#DEPENDPATH += $$PWD/qwt-6.1.2/src/

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/qwt-6.1.2/lib/ -lqwtmathml
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/qwt-6.1.2/lib/ -lqwtmathmld

#INCLUDEPATH += $$PWD/qwt-6.1.2/textengines/mathml/
#DEPENDPATH += $$PWD/qwt-6.1.2/textengines/mathml/
