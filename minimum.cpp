#include "minimum.h"
#include <QtMath>
#include <QtGui/QStandardItemModel>
#include <QDebug>

Minimum::Minimum(QStandardItemModel *model)
{
    model_ = model;
    isOvi_ = false;
    isDay_ = false;
    isLights_ = false;
    heightObstacle_ = 0;
    rangeVisibility_["A-Lights"] = 200;
    rangeVisibility_["B-Lights"] = 200;
    rangeVisibility_["C-Lights"] = 200;
    rangeVisibility_["D-Lights"] = 200;
    rangeVisibility_["A-NotLights-Ovi-Day"] = 300;
    rangeVisibility_["B-NotLights-Ovi-Day"] = 300;
    rangeVisibility_["C-NotLights-Ovi-Day"] = 300;
    rangeVisibility_["D-NotLights-Ovi-Day"] = 300;
    rangeVisibility_["A-NotLights-Ovi-Night"] = 300;
    rangeVisibility_["B-NotLights-Ovi-Night"] = 300;
    rangeVisibility_["C-NotLights-Ovi-Night"] = 400;
    rangeVisibility_["D-NotLights-Ovi-Night"] = 400;
    rangeVisibility_["A-NotLights-NotOvi-Day"] = 300;
    rangeVisibility_["B-NotLights-NotOvi-Day"] = 300;
    rangeVisibility_["C-NotLights-NotOvi-Day"] = 500;
    rangeVisibility_["D-NotLights-NotOvi-Day"] = 500;
    rangeVisibility_["A-NotLights-NotOvi-Night"] = 300;
    rangeVisibility_["B-NotLights-NotOvi-Night"] = 300;
    rangeVisibility_["C-NotLights-NotOvi-Night"] = 700;
    rangeVisibility_["D-NotLights-NotOvi-Night"] = 700;

    minimumLanding_["H-SP.ILS-Cat-2"] = qMakePair(30, 400);
    minimumLanding_["A-SP.ILS-Cat-2"] = qMakePair(30, 350);
    minimumLanding_["B-SP.ILS-Cat-2"] = qMakePair(30, 350);
    minimumLanding_["C-SP.ILS-Cat-2"] = qMakePair(30, 350);
    minimumLanding_["D-SP.ILS-Cat-2"] = qMakePair(30, 350);

    minimumLanding_["H-SP.ILS-Cat-1"] = qMakePair(60, 600);
    minimumLanding_["A-SP.ILS-Cat-1"] = qMakePair(60, 550);
    minimumLanding_["B-SP.ILS-Cat-1"] = qMakePair(60, 550);
    minimumLanding_["C-SP.ILS-Cat-1"] = qMakePair(60, 550);
    minimumLanding_["D-SP.ILS-Cat-1"] = qMakePair(60, 550);

    minimumLanding_["H-SP.ILS-Cat-PSP"] = qMakePair(60, 600);
    minimumLanding_["A-SP.ILS-Cat-PSP"] = qMakePair(60, 800);
    minimumLanding_["B-SP.ILS-Cat-PSP"] = qMakePair(60, 800);
    minimumLanding_["C-SP.ILS-Cat-PSP"] = qMakePair(80, 1000);
    minimumLanding_["D-SP.ILS-Cat-PSP"] = qMakePair(80, 1000);

    minimumLanding_["H-SP.ILS-NotCat"] = qMakePair(60, 600);
    minimumLanding_["A-SP.ILS-NotCat"] = qMakePair(60, 800);
    minimumLanding_["B-SP.ILS-NotCat"] = qMakePair(60, 800);
    minimumLanding_["C-SP.ILS-NotCat"] = qMakePair(70, 900);
    minimumLanding_["D-SP.ILS-NotCat"] = qMakePair(70, 900);

    minimumLanding_["H-PSP"] = qMakePair(60, 600);
    minimumLanding_["A-PSP"] = qMakePair(60, 800);
    minimumLanding_["B-PSP"] = qMakePair(60, 800);
    minimumLanding_["C-PSP"] = qMakePair(80, 1000);
    minimumLanding_["D-PSP"] = qMakePair(80, 1000);

    minimumLanding_["H-RMS-PSP"] = qMakePair(60, 600);
    minimumLanding_["A-RMS-PSP"] = qMakePair(60, 800);
    minimumLanding_["B-RMS-PSP"] = qMakePair(60, 800);
    minimumLanding_["C-RMS-PSP"] = qMakePair(80, 1000);
    minimumLanding_["D-RMS-PSP"] = qMakePair(80, 1000);

    minimumLanding_["H-RSP.OSP"] = qMakePair(60, 600);
    minimumLanding_["A-RSP.OSP"] = qMakePair(60, 800);
    minimumLanding_["B-RSP.OSP"] = qMakePair(80, 1000);
    minimumLanding_["C-RSP.OSP"] = qMakePair(80, 1000);
    minimumLanding_["D-RSP.OSP"] = qMakePair(80, 1000);

    minimumLanding_["H-RSP"] = qMakePair(60, 600);
    minimumLanding_["A-RSP"] = qMakePair(60, 800);
    minimumLanding_["B-RSP"] = qMakePair(80, 1000);
    minimumLanding_["C-RSP"] = qMakePair(80, 1000);
    minimumLanding_["D-RSP"] = qMakePair(100, 1200);

    minimumLanding_["H-OSP"] = qMakePair(80, 800);
    minimumLanding_["A-OSP"] = qMakePair(80, 1000);
    minimumLanding_["B-OSP"] = qMakePair(100, 1500);
    minimumLanding_["C-OSP"] = qMakePair(100, 1500);
    minimumLanding_["D-OSP"] = qMakePair(100, 1500);
}

Minimum::~Minimum()
{
    rangeVisibility_.clear();
}

void Minimum::getMinimumTakeoff()
{
    QStringList categoryAircraft;
    categoryAircraft << "A" << "B" << "C" << "D";

    QString key = QString();

    if (isLights_)
        key += "-Lights";
    else {
        key += "-NotLights";
        if (isOvi_)
            key += "-Ovi";
        else
            key += "-NotOvi";
        if (isDay_)
            key += "-Day";
        else
            key += "-Night";
    }

    int row =1;
    QStringList::const_iterator constIt;
    for (constIt = categoryAircraft.constBegin(); constIt != categoryAircraft.constEnd(); ++constIt) {
        int heightNGO = computeHeightNgoForTakeoff((*constIt));
        int rangeVisibility = computeRangeVisibility(heightNGO);
        int rangeVisibilityTable = rangeVisibility_.value((*constIt) + key);
        rangeVisibility = rangeVisibilityTable > rangeVisibility ? rangeVisibilityTable : rangeVisibility;

        QString text = QString("%1x%2").arg(heightNGO).arg(rangeVisibility);
        if (model_->item(row, 1))
            model_->item(row, 1)->setText(text);
        else
            model_->setItem(row, 1, new QStandardItem(text));
        row++;
    }
}

int Minimum::computeHeightNgoForTakeoff(const QString &type)
{
    int deltaHeight = 0;
    if (type.contains(QRegExp("[CDE]")))
        deltaHeight = 50;
    else
        deltaHeight = 30;         // type aircraft - A, B

    int h = qCeil(heightObstacle_ + deltaHeight);
    qDebug() << h;
    return h;
}

int Minimum::computeRangeVisibility(int heightNGO)
{
    return qCeil(6 * heightNGO + 300);
}

void Minimum::getMinimumLanding()
{
    QStringList categoryAircraft;
    categoryAircraft << "H" << "A" << "B" << "C" << "D";
    QPair<int, int> min;

    int heightPR = 0;
    int heightNGO = 0;
    int row = 0;
    int rangeVisibility = 0;
    QStringList::const_iterator constIt;
    for (constIt = categoryAircraft.constBegin(); constIt != categoryAircraft.constEnd(); ++constIt) {
        min = minimumLanding_.value((*constIt) + "-SP.ILS-Cat-1");
        heightNGO = computeHeightNgoForTakeoff((*constIt));
        heightPR = heightNGO > min.first ? heightNGO : min.first;

        QString text = QString("%1x%2").arg(heightPR).arg(min.second);
        if (model_->item(row, 2))
            model_->item(row, 2)->setText(text);
        else
            model_->setItem(row, 2, new QStandardItem(text));

        min = minimumLanding_.value((*constIt) + "-PSP");
        heightNGO = computeHeightNgoForTakeoff((*constIt));
        if (heightNGO > min.first)
            heightPR = heightNGO;
        else
            heightPR = min.first;


        text = QString("%1x%2").arg(heightPR).arg(min.second);
        if (model_->item(row, 3))
            model_->item(row, 3)->setText(text);
        else
            model_->setItem(row, 3, new QStandardItem(text));

        min = minimumLanding_.value((*constIt) + "-RMS-PSP");
        heightNGO = computeHeightNgoForTakeoff((*constIt));
        heightPR = heightNGO > min.first ? heightNGO : min.first;

        text = QString("%1x%2").arg(heightPR).arg(min.second);
        if (model_->item(row, 4))
            model_->item(row, 4)->setText(text);
        else
            model_->setItem(row, 4, new QStandardItem(text));

        min = minimumLanding_.value((*constIt) + "-RSP.OSP");
        heightNGO = computeHeightNgoForTakeoff((*constIt));
        rangeVisibility = min.second;
        if (heightNGO > min.first) {
            heightPR = heightNGO;

            if (heightPR >= 60 && heightPR <=65)
                rangeVisibility = 800;
            else if (heightPR >= 70 && heightPR <= 75)
                rangeVisibility = 900;
            else if (heightPR >= 80 && heightPR <= 85)
                rangeVisibility = 1000;
            else if (heightPR >= 90 && heightPR <= 95)
                rangeVisibility = 1100;
            else if (heightPR >= 100 && heightPR <= 105)
                rangeVisibility = 1200;
            else if (heightPR >= 110 && heightPR <= 115)
                rangeVisibility = 1400;
            else if (heightPR >= 120 && heightPR <= 125)
                rangeVisibility = 1500;
        }
        else
            heightPR = min.first;

        text = QString("%1x%2").arg(heightPR).arg(rangeVisibility);
        if (model_->item(row, 5))
            model_->item(row, 5)->setText(text);
        else
            model_->setItem(row, 5, new QStandardItem(text));

        min = minimumLanding_.value((*constIt) + "-RSP");
        heightNGO = computeHeightNgoForTakeoff((*constIt));
        rangeVisibility = min.second;
        if (heightNGO > min.first) {
            heightPR = heightNGO;
            if (heightPR >= 60 && heightPR<=65)
                rangeVisibility = 800;
            else if (heightPR >= 70 && heightPR <= 75)
                rangeVisibility = 900;
            else if (heightPR >= 80 && heightPR <= 85)
                rangeVisibility = 1000;
            else if (heightPR >= 90 && heightPR <= 95)
                rangeVisibility = 1100;
            else if (heightPR >= 100 && heightPR <= 105)
                rangeVisibility = 1200;
            else if (heightPR >= 110 && heightPR <= 115)
                rangeVisibility = 1400;
            else if (heightPR >= 120 && heightPR <= 125)
                rangeVisibility = 1500;
        }
        else
            heightPR = min.first;

        text = QString("%1x%2").arg(heightPR).arg(rangeVisibility);
        if (model_->item(row, 6))
            model_->item(row, 6)->setText(text);
        else
            model_->setItem(row, 6, new QStandardItem(text));

        min = minimumLanding_.value((*constIt) + "-OSP");
        heightNGO = computeHeightNgoForTakeoff((*constIt));
        rangeVisibility = min.second;
        if (heightNGO > min.first) {
            heightPR = heightNGO;

            if (heightPR >= 80 && heightPR <= 85)
                rangeVisibility = 1200;
            else if (heightPR >= 90 && heightPR <= 95)
                rangeVisibility = 1400;
            else if (heightPR >= 100 && heightPR <= 105)
                rangeVisibility = 1500;
            else if (heightPR >= 110 && heightPR <= 115)
                rangeVisibility = 1700;
            else if (heightPR >= 120 && heightPR <= 125)
                rangeVisibility = 1800;
        }
        else
            heightPR = min.first;

        text = QString("%1x%2").arg(heightPR).arg(rangeVisibility);
        if (model_->item(row, 7))
            model_->item(row, 7)->setText(text);
        else
            model_->setItem(row, 7, new QStandardItem(text));
        row++;
    }
}

void Minimum::computeMinimum()
{
    getMinimumTakeoff();
    getMinimumLanding();
}
