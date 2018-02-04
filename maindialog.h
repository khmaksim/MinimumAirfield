#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QtWidgets/QDialog>
#include <QtCore/QMap>

namespace Ui {
    class MainDialog;
}

class MainDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit MainDialog(QWidget *parent = 0);
        ~MainDialog();

    private:
        Ui::MainDialog *ui;
        QStringList numbersThresholds;
        QMap<QString, int> categoryAircraft;

        void readSettings();
        void writeSettings();
        int round(double height, int base);

    private slots:
        void computeTakeoffMinimum();
        void setNumbersThresholds(const QString &text);
        void reset();
        void setNumbersThresholdsIntoTable();
};

#endif // MAINDIALOG_H
