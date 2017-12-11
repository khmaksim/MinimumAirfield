#include "maindialog.h"
#include <QApplication>
#include <QtCore/QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("Aviacominfo");
    QCoreApplication::setOrganizationDomain("aviacominfo.com");
    QCoreApplication::setApplicationName("Minimum");

    QTranslator translator;
    if (translator.load(QLocale(), QLatin1String("minimumairfield"), QLatin1String("_"), QLatin1String(":/translations")))
        a.installTranslator(&translator);

    MainDialog w;
    w.show();

    return a.exec();
}
