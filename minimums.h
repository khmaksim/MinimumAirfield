#ifndef MINIMUMS_H
#define MINIMUMS_H

#include <QMap>

class Minimums
{
    public:
        Minimums();
        ~Minimums();

        int getMinimumsWithLights(const QString &type);
        int getMinimumsWithoutLightsWithOviDay(const QString &type);
        int getMinimumsWithoutLightsWithOviNight(const QString &type);
        int getMinimumsWithoutLightsWithoutOviDay(const QString &type);
        int getMinimumsWithoutLightsWithoutOviNight(const QString &type);

    private:
        QMap<QString, int> minimumsWithLights;
        QMap<QString, int> minimumsWithoutLightsWithOviDay;
        QMap<QString, int> minimumsWithoutLightsWithOviNight;
        QMap<QString, int> minimumsWithoutLightsWithoutOviDay;
        QMap<QString, int> minimumsWithoutLightsWithoutOviNight;
};

#endif // MINIMUMS_H
