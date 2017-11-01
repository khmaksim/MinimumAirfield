#include "minimums.h"

Minimums::Minimums()
{
    minimumsWithLights["A"] = 200;
    minimumsWithLights["B"] = 200;
    minimumsWithLights["C"] = 200;
    minimumsWithLights["D"] = 200;
    minimumsWithLights["E"] = 200;
    minimumsWithoutLightsWithOviDay["A"] = 300;
    minimumsWithoutLightsWithOviDay["B"] = 300;
    minimumsWithoutLightsWithOviDay["C"] = 300;
    minimumsWithoutLightsWithOviDay["D"] = 300;
    minimumsWithoutLightsWithOviDay["E"] = 300;
    minimumsWithoutLightsWithOviNight["A"] = 300;
    minimumsWithoutLightsWithOviNight["B"] = 300;
    minimumsWithoutLightsWithOviNight["C"] = 400;
    minimumsWithoutLightsWithOviNight["D"] = 400;
    minimumsWithoutLightsWithOviNight["E"] = 400;
    minimumsWithoutLightsWithoutOviDay["A"] = 300;
    minimumsWithoutLightsWithoutOviDay["B"] = 300;
    minimumsWithoutLightsWithoutOviDay["C"] = 500;
    minimumsWithoutLightsWithoutOviDay["D"] = 500;
    minimumsWithoutLightsWithoutOviDay["E"] = 500;
    minimumsWithoutLightsWithoutOviNight["A"] = 300;
    minimumsWithoutLightsWithoutOviNight["B"] = 300;
    minimumsWithoutLightsWithoutOviNight["C"] = 700;
    minimumsWithoutLightsWithoutOviNight["D"] = 700;
    minimumsWithoutLightsWithoutOviNight["E"] = 700;
}

Minimums::~Minimums()
{
    minimumsWithLights.clear();
    minimumsWithoutLightsWithOviDay.clear();
    minimumsWithoutLightsWithOviNight.clear();
    minimumsWithoutLightsWithoutOviDay.clear();
    minimumsWithoutLightsWithoutOviNight.clear();
}

int Minimums::getMinimumsWithLights(const QString &type)
{
    return minimumsWithLights.value(type);
}

int Minimums::getMinimumsWithoutLightsWithOviDay(const QString &type)
{
    return minimumsWithoutLightsWithOviDay.value(type);
}

int Minimums::getMinimumsWithoutLightsWithOviNight(const QString &type)
{
    return minimumsWithoutLightsWithOviNight.value(type);
}

int Minimums::getMinimumsWithoutLightsWithoutOviDay(const QString &type)
{
    return minimumsWithoutLightsWithoutOviDay.value(type);
}

int Minimums::getMinimumsWithoutLightsWithoutOviNight(const QString &type)
{
    return minimumsWithoutLightsWithoutOviNight.value(type);
}
