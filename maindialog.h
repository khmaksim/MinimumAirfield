#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QtWidgets/QDialog>

namespace Ui {
    class MainDialog;
}

class Minimum;
class QStandardItemModel;
class MainDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit MainDialog(QWidget *parent = 0);
        ~MainDialog();

    private:
        Ui::MainDialog *ui;
        QStandardItemModel *model;
        Minimum *minimum;

        void getListAirfields();
        void readSettings();
        void writeSettings();
        bool connectDatabase();
        void fillTable();

    private slots:
        void setLights();
        void setTimeOfDay();
        void setOvi();
        void getListObstaclesByAirfield(int);
};

#endif // MAINDIALOG_H
