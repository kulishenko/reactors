#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QDebug>
#include <math.h>

int main(int argc, char *argv[])
{
    QStringList paths = QCoreApplication::libraryPaths();
      paths.append(".");
  //    paths.append("imageformats");
      paths.append("platforms");
      paths.append("sqldrivers");
      QCoreApplication::setLibraryPaths(paths);

    QApplication a(argc, argv);
//#define STYLE
#ifdef STYLE
    QFile file(":/resources/dark.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
   // a.setStyle("plastique");
    a.setStyleSheet(styleSheet);
    file.close();
#endif
a.setStyle("fusion");
    QTranslator qtTranslator;
       qtTranslator.load("qt_" + QLocale::system().name(),
               QLibraryInfo::location(QLibraryInfo::TranslationsPath));
       a.installTranslator(&qtTranslator);


    QTranslator myappTranslator;
    myappTranslator.load(":/Reactors_" + QLocale::system().name());
    a.installTranslator(&myappTranslator);

    MainWindow w;
    w.show();
    return a.exec();
}

