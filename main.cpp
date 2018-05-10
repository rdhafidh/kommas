#include "mainwindow.h"
#include "crash_handler.h"
#include <QApplication>
#include <QDir>
#include <QtCrypto>
#include <QStyleFactory>
#include <QDebug>

int main(int argc, char *argv[])
{
  QCA::Initializer init;
    QApplication a(argc, argv);
    a.addLibraryPath(".");
	a.addLibraryPath("style");
    QApplication::setStyle(QStyleFactory::create("cleanlooks"));

    Breakpad::CrashHandler::instance()->Init(QDir::currentPath());
    MainWindow w;
    w.show();

    return a.exec();
}
