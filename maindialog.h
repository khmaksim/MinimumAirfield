#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QtWidgets/QDialog>

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

        void readSettings();
        void writeSettings();
        int round(double height, int base);

    private slots:
        void computeTakeoffMinimum();
};

#endif // MAINDIALOG_H
