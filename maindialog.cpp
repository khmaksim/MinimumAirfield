#include "maindialog.h"
#include "ui_maindialog.h"
#include <QtWidgets/QMessageBox>
#include <QtCore/QSettings>
#include <QDebug>

MainDialog::MainDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainDialog)
{
    ui->setupUi(this);

//    readSettings();

    categoryAircraft.insert("H", 0);
    categoryAircraft.insert("A", 1);
    categoryAircraft.insert("B", 2);
    categoryAircraft.insert("C", 3);
    categoryAircraft.insert("D", 4);
    categoryAircraft.insert("E", 5);

    ui->numberThreshold1LineEdit->setValidator(new QIntValidator(1, 19));
    ui->numberThreshold2LineEdit->setValidator(new QIntValidator(19, 36));

    ui->minimumTakeoffTableWidget->setItem(0, 0, new QTableWidgetItem(tr("MKvzk")));
    ui->minimumTakeoffTableWidget->setItem(0, 1, new QTableWidgetItem(tr("helicopters")));
    ui->minimumTakeoffTableWidget->setItem(0, 2, new QTableWidgetItem(tr("A")));
    ui->minimumTakeoffTableWidget->setItem(0, 3, new QTableWidgetItem(tr("B")));
    ui->minimumTakeoffTableWidget->setItem(0, 4, new QTableWidgetItem(tr("C")));
    ui->minimumTakeoffTableWidget->setItem(0, 5, new QTableWidgetItem(tr("D")));
    ui->minimumTakeoffTableWidget->setItem(0, 6, new QTableWidgetItem(tr("E")));
    ui->minimumTakeoffTableWidget->setItem(1, 0, new QTableWidgetItem(tr("Day, Hngo x Lv")));
    ui->minimumTakeoffTableWidget->setItem(2, 0, new QTableWidgetItem(tr("Night, Hngo x Lv")));

    QObjectList checkTypeList = ui->categoryAircraftGroupBox->children();
    for (int i = 0; i < checkTypeList.size(); i++) {
        QCheckBox *obj = qobject_cast<QCheckBox*>(checkTypeList.at(i));
        if (obj)
            connect(obj, SIGNAL(clicked(bool)), this, SLOT(computeTakeoffMinimum()));
    }
    connect(ui->numberThreshold1LineEdit, SIGNAL(textChanged(QString)), this, SLOT(setNumbersThresholds(QString)));
    connect(ui->numberThreshold2LineEdit, SIGNAL(textChanged(QString)), this, SLOT(setNumbersThresholds(QString)));
    connect(ui->ivppRadioButton, SIGNAL(clicked(bool)), this, SLOT(computeTakeoffMinimum()));
    connect(ui->alternateAirportCheckBox, SIGNAL(clicked(bool)), this, SLOT(computeTakeoffMinimum()));
    connect(ui->dataLoggerCheckBox, SIGNAL(clicked(bool)), this, SLOT(computeTakeoffMinimum()));
    connect(ui->withLightsCenterlineRadioButton, SIGNAL(clicked(bool)), this, SLOT(computeTakeoffMinimum()));
    connect(ui->withoutLightsCenterlineRadioButton, SIGNAL(clicked(bool)), this, SLOT(computeTakeoffMinimum()));
    connect(ui->markingCenterlineCheckBox, SIGNAL(clicked(bool)), this, SLOT(computeTakeoffMinimum()));
    connect(ui->withSideLightsRadioButton, SIGNAL(clicked(bool)), this, SLOT(computeTakeoffMinimum()));
    connect(ui->dayRadioButton, SIGNAL(clicked(bool)), this, SLOT(computeTakeoffMinimum()));
    connect(ui->nightRadioButton, SIGNAL(clicked(bool)), this, SLOT(computeTakeoffMinimum()));
    connect(ui->withoutSideLightsRadioButton, SIGNAL(clicked(bool)), this, SLOT(computeTakeoffMinimum()));
    connect(ui->withoutLightsCenterlineRadioButton, SIGNAL(toggled(bool)), ui->frameSideLight, SLOT(setEnabled(bool)));
    connect(ui->withoutLightsCenterlineRadioButton, SIGNAL(toggled(bool)), ui->frameTimeDay, SLOT(setEnabled(bool)));
}

MainDialog::~MainDialog()
{
    writeSettings();
    delete ui;
}

void MainDialog::readSettings()
{
    QSettings settings;

    settings.beginGroup("geometry");
    settings.endGroup();
}

void MainDialog::writeSettings()
{
    QSettings settings;

    settings.beginGroup("geometry");
    settings.endGroup();
}

void MainDialog::computeTakeoffMinimum()
{
    double mk1Height = ui->mk1HightObstacleDoubleSpinBox->value();
    double mk2Height = ui->mk2HightObstacleDoubleSpinBox->value();
    double heightObstacle = 0;
    double visibility_obstacle = 0;      // видимость при наличии препятствия
    double visibility = 0;               // видимость
    double height = 0;                   // высота нижней границы облаков


    QObjectList checkTypeList = ui->categoryAircraftGroupBox->children();
    for (int i = 0; i < checkTypeList.size(); i++) {
        QCheckBox *obj = qobject_cast<QCheckBox*>(checkTypeList.at(i));
        if (obj && obj->isChecked()) {
            int indexCategoryAircraft = categoryAircraft.value(obj->text());

            if (mk1Height != 0 || mk2Height != 0) {
                heightObstacle = qMax(mk1Height, mk2Height);

                double shift = 0;
                switch (indexCategoryAircraft) {
                    case 0:
                    case 1:
                    case 2:
                        shift = 30;
                        break;
                    case 3:
                    case 4:
                    case 5:
                        shift = 50;
                        break;
                }
                height = heightObstacle + shift;
                visibility_obstacle = 6 * round(height, 10) + 300;
                visibility_obstacle = round(visibility_obstacle, 100);
            }

            // Видимость
            // ИВПП
            if (ui->ivppRadioButton->isChecked()) {
                // Запасной аэродром
                if (ui->alternateAirportCheckBox->isChecked()) {
                    // Наличие РДВ
                    if (ui->dataLoggerCheckBox->isChecked()) {
                        // С огнями осевой линии
                        if (ui->withLightsCenterlineRadioButton->isChecked()) {
                            switch (indexCategoryAircraft) {
                                case 0:
                                    break;
                                case 1:
                                case 2:
                                case 3:
                                case 4:
                                case 5:
                                    visibility = 200;
                                    break;
                            }
                        }
                        // Без огней осевой линии с маркеровкрой осевой линии
                        else if (ui->withoutLightsCenterlineRadioButton->isChecked() &&
                                 ui->markingCenterlineCheckBox->isChecked()) {
                            //  С ОВИ (ОМИ)
                            if (ui->withSideLightsRadioButton->isChecked()) {
                                // День
                                if (ui->dayRadioButton->isChecked()) {
                                    switch (indexCategoryAircraft) {
                                        case 0:
                                            break;
                                        case 1:
                                        case 2:
                                        case 3:
                                        case 4:
                                        case 5:
                                            visibility = 300;
                                            break;
                                    }
                                }
                                // Ночь
                                else if (ui->nightRadioButton->isChecked()) {
                                    switch (indexCategoryAircraft) {
                                        case 0:
                                            break;
                                        case 1:
                                        case 2:
                                            visibility = 300;
                                            break;
                                        case 3:
                                        case 4:
                                        case 5:
                                            visibility = 400;
                                            break;
                                    }
                                }
                            }
                            // Без ОВИ (ОМИ)
                            else if (ui->withoutSideLightsRadioButton->isChecked()) {
                                // День
                                if (ui->dayRadioButton->isChecked()) {
                                    switch (indexCategoryAircraft) {
                                        case 0:
                                            break;
                                        case 1:
                                        case 2:
                                            visibility = 300;
                                            break;
                                        case 3:
                                        case 4:
                                        case 5:
                                            visibility = 500;
                                            break;
                                    }
                                }
                                // Ночь
                                else if (ui->nightRadioButton->isChecked()) {
                                    switch (indexCategoryAircraft) {
                                        case 0:
                                            break;
                                        case 1:
                                        case 2:
                                            visibility = 300;
                                            break;
                                        case 3:
                                        case 4:
                                        case 5:
                                            visibility = 700;
                                            break;
                                    }
                                }
                            }
                        }
                    }
                    // Без РДВ
                    else {
                        // Без ОВИ (ОМИ)
                        if (ui->withoutSideLightsRadioButton->isChecked()) {
                            // День
                            if (ui->dayRadioButton->isChecked()) {
                                switch (indexCategoryAircraft) {
                                    case 0:
                                        break;
                                    case 1:
                                    case 2:
                                        visibility = 300;
                                        break;
                                    case 3:
                                    case 4:
                                    case 5:
                                        visibility = 500;
                                        break;
                                }
                            }
                            // Ночь
                            else if (ui->nightRadioButton->isChecked()) {
                                switch (indexCategoryAircraft) {
                                    case 0:
                                        break;
                                    case 1:
                                    case 2:
                                        visibility = 300;
                                        break;
                                    case 3:
                                    case 4:
                                    case 5:
                                        visibility = 700;
                                        break;
                                }
                            }
                        }
                    }
                }
            }
            // ГВПП
            else if (ui->gvppRadioButton->isChecked()) {
                if (ui->markingCenterlineCheckBox->isChecked() || ui->withLightsCenterlineRadioButton) {
                    // С огнями осевой линии
                    if (ui->withLightsCenterlineRadioButton->isChecked()) {
                        switch (indexCategoryAircraft) {
                            case 0:
                                break;
                            case 1:
                            case 2:
                            case 3:
                            case 4:
                            case 5:
                                visibility = 200;
                                break;
                        }
                    }
                    // Без огней осевой линии с маркеровкрой осевой линии
                    else if (ui->withoutLightsCenterlineRadioButton->isChecked() &&
                             ui->markingCenterlineCheckBox->isChecked()) {
                        //  С ОВИ (ОМИ)
                        if (ui->withSideLightsRadioButton->isChecked()) {
                            // День
                            if (ui->dayRadioButton->isChecked()) {
                                switch (indexCategoryAircraft) {
                                    case 0:
                                        break;
                                    case 1:
                                    case 2:
                                    case 3:
                                    case 4:
                                    case 5:
                                        visibility = 300;
                                        break;
                                }
                            }
                            // Ночь
                            else if (ui->nightRadioButton->isChecked()) {
                                switch (indexCategoryAircraft) {
                                    case 0:
                                        break;
                                    case 1:
                                    case 2:
                                        visibility = 300;
                                        break;
                                    case 3:
                                    case 4:
                                    case 5:
                                        visibility = 400;
                                        break;
                                }
                            }
                        }
                        // Без ОВИ (ОМИ)
                        else if (ui->withoutSideLightsRadioButton->isChecked()) {
                            // День
                            if (ui->dayRadioButton->isChecked()) {
                                switch (indexCategoryAircraft) {
                                    case 0:
                                        break;
                                    case 1:
                                    case 2:
                                        visibility = 300;
                                        break;
                                    case 3:
                                    case 4:
                                    case 5:
                                        visibility = 500;
                                        break;
                                }
                            }
                            // Ночь
                            else if (ui->nightRadioButton->isChecked()) {
                                switch (indexCategoryAircraft) {
                                    case 0:
                                        break;
                                    case 1:
                                    case 2:
                                        visibility = 300;
                                        break;
                                    case 3:
                                    case 4:
                                    case 5:
                                        visibility = 700;
                                        break;
                                }
                            }
                        }
                    }
                }
                else {
                    // минимумы для взлёта == минимумам для посадки
                }
            }
            visibility = visibility_obstacle < visibility ? visibility : visibility_obstacle;

            QString result = QString("%1 x %2").arg(height).arg(visibility);

            // if night then result insert to second row
            int row = 1;
            if (ui->nightRadioButton->isChecked())
                row = 2;

            switch (indexCategoryAircraft) {
                case 0:
                    ui->minimumTakeoffTableWidget->setItem(row, 1, new QTableWidgetItem(result));
                    break;
                case 1:
                    ui->minimumTakeoffTableWidget->setItem(row, 2, new QTableWidgetItem(result));
                    break;
                case 2:
                    ui->minimumTakeoffTableWidget->setItem(row, 3, new QTableWidgetItem(result));
                    break;
                case 3:
                    ui->minimumTakeoffTableWidget->setItem(row, 4, new QTableWidgetItem(result));
                    break;
                case 4:
                    ui->minimumTakeoffTableWidget->setItem(row, 5, new QTableWidgetItem(result));
                    break;
                case 5:
                    ui->minimumTakeoffTableWidget->setItem(row, 6, new QTableWidgetItem(result));
                    break;
            }
        }
    }
}

int MainDialog::round(double height, int base)
{
    int h = qRound(height);

    int a = h % base;

    if (a > 0)
        return h = h + (base - a);
    else
        return h;
}

void MainDialog::setNumbersThresholds(const QString &text)
{
    if (text.length() < 2)
        return;

    QString number = text;

    if (sender()->objectName().contains("numberThreshold1")) {
        ui->numberThreshold2LineEdit->blockSignals(true);
        number = QString::number(number.toInt() + 18);
        ui->numberThreshold2LineEdit->setText(number);
        ui->numberThreshold2LineEdit->blockSignals(false);
    }
    else {
        ui->numberThreshold1LineEdit->blockSignals(true);
        number = QString::number(number.toInt() - 18);
        if (number.length() < 2)
            number = number.rightJustified(2, '0');
        ui->numberThreshold1LineEdit->setText(number);
        ui->numberThreshold1LineEdit->blockSignals(false);
    }

    //    QStringList numbers = text.split("/");
    //    QString num1 = "00";
    //    QString num2 = "00";

    //    if (numbersThresholds.isEmpty() || numbersThresholds.count() < 2)
    //        numbersThresholds = numbers;

    //    if (numbersThresholds[0] != numbers[0]) {
    //        num1 = numbers[0];
    //        num2 = QString().append(num1[1]).append(num1[0]);

    //    }
    //    if (numbersThresholds[1] != numbers[1]) {
    //        num2 = numbers[1];
    //        num2 = QString().append(num2[1]).append(num2[0]);
    //    }
    return;
}
