#include "dialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    QTranslator translator;
//    if (translator.load(QLocale(), QLatin1String("reportjob"), QLatin1String("_"), QLatin1String(":/translations")))
//        a.installTranslator(&translator);

    Dialog w;
    w.show();

    return a.exec();
}
