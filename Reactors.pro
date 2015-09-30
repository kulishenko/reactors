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
    libxls/src/xlstool.c

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
    libxls/include/libxls/config.h

FORMS    += mainwindow.ui

TRANSLATIONS    = Reactors_ru.ts \
                  Reactors_de.ts




win32: LIBS += -L$$PWD/libxls/lib/ -lxlsreader

INCLUDEPATH += $$PWD/libxls/include
DEPENDPATH += $$PWD/libxls/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/libxls/lib/xlsreader.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/libxls/lib/libxlsreader.a

RESOURCES += \
    reactors.qrc

DISTFILES +=

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/qcustomplot/build-sharedlib-compilation-Desktop_Qt_5_4_1_MinGW_32bit-Release/release/ -lqcustomplot1
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/qcustomplot/build-sharedlib-compilation-Desktop_Qt_5_4_1_MinGW_32bit-Release/debug/ -lqcustomplotd1

INCLUDEPATH += $$PWD/qcustomplot/build-sharedlib-compilation-Desktop_Qt_5_4_1_MinGW_32bit-Release/release
DEPENDPATH += $$PWD/qcustomplot/build-sharedlib-compilation-Desktop_Qt_5_4_1_MinGW_32bit-Release/release

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/qcustomplot/build-sharedlib-compilation-Desktop_Qt_5_4_1_MinGW_32bit-Release/release/libqcustomplot1.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/qcustomplot/build-sharedlib-compilation-Desktop_Qt_5_4_1_MinGW_32bit-Release/debug/libqcustomplotd1.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/qcustomplot/build-sharedlib-compilation-Desktop_Qt_5_4_1_MinGW_32bit-Release/release/qcustomplot1.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/qcustomplot/build-sharedlib-compilation-Desktop_Qt_5_4_1_MinGW_32bit-Release/debug/qcustomplotd1.lib
