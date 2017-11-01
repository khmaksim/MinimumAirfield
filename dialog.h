#ifndef DIALOG_H
#define DIALOG_H

#include <QtWidgets/QDialog>

namespace Ui {
    class Dialog;
}

class Minimums;
class Dialog : public QDialog
{
        Q_OBJECT

    public:
        explicit Dialog(QWidget *parent = 0);
        ~Dialog();

    private:
        Ui::Dialog *ui;
        Minimums *minimums;

        bool readData();
        void readSettings();

    private slots:
        void computeMinimums();
};

#endif // DIALOG_H
