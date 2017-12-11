#ifndef MINIMUM_H
#define MINIMUM_H

#include <QMap>

class QStandardItemModel;
class Minimum
{
    public:
        Minimum(QStandardItemModel *model);
        ~Minimum();

        void getMinimumTakeoff();
        void getMinimumLanding();
        void setHeightObstacle(float value) { heightObstacle_ = value; }
        void setOvi(bool flag) { isOvi_ = flag; }
        void setTimeOfDay(bool flag) { isDay_ = flag; }
        void setLights(bool flag) { isLights_ = flag; }
        void computeMinimum();

    private:
        QMap<QString, int> rangeVisibility_;
        QMap<QString, QPair<int, int>> minimumLanding_;
        float heightObstacle_;
        bool isOvi_;
        bool isDay_;
        bool isLights_;
        QStandardItemModel *model_;

        int computeHeightNgoForTakeoff(const QString &type);
        int computeRangeVisibility(int);
};

#endif // MINIMUM_H
