#include "maindialog.h"
#include "ui_maindialog.h"
#include <QtWidgets/QMessageBox>
#include <QtCore/QSettings>
#include <QDebug>
#include "qgroupheaderview.h"

MainDialog::MainDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(windowFlags() | Qt::WindowMinMaxButtonsHint);

    categoryAircraft.insert("H", 0);
    categoryAircraft.insert("A", 1);
    categoryAircraft.insert("B", 2);
    categoryAircraft.insert("C", 3);
    categoryAircraft.insert("D", 4);
    categoryAircraft.insert("E", 5);

    QGroupHeaderView *groupHeaderView = new QGroupHeaderView(Qt::Horizontal, ui->minimumTakeoffTableWidget);
    ui->minimumTakeoffTableWidget->setHorizontalHeaderLabels(QStringList()
                                                             << tr("The threshold VPP")
                                                             << tr("Day/Night")
                                                             << tr("Category aircraft | helicopters | (Hngo x Lvid), m")
                                                             << tr("Category aircraft | A | (Hngo x Lvid), m")
                                                             << tr("Category aircraft | B | (Hngo x Lvid), m")
                                                             << tr("Category aircraft | C | (Hngo x Lvid), m")
                                                             << tr("Category aircraft | D | (Hngo x Lvid), m")
                                                             << tr("Category aircraft | E | (Hngo x Lvid), m"));

    ui->minimumTakeoffTableWidget->setHorizontalHeader(groupHeaderView);
    ui->numberThreshold1LineEdit->setValidator(new QIntValidator(1, 19));
    ui->numberThreshold2LineEdit->setValidator(new QIntValidator(19, 36));

    ui->minimumTakeoffTableWidget->setItem(0, 1, new QTableWidgetItem(tr("Day")));
    ui->minimumTakeoffTableWidget->setItem(1, 1, new QTableWidgetItem(tr("Night")));
    ui->minimumTakeoffTableWidget->setItem(2, 1, new QTableWidgetItem(tr("Day")));
    ui->minimumTakeoffTableWidget->setItem(3, 1, new QTableWidgetItem(tr("Night")));

    QObjectList checkTypeList = ui->categoryAircraftGroupBox->children();
    for (int i = 0; i < checkTypeList.size(); i++) {
        QCheckBox *obj = qobject_cast<QCheckBox*>(checkTypeList.at(i));
        if (obj)
            connect(obj, SIGNAL(clicked(bool)), this, SLOT(computeTakeoffMinimum()));
    }

    readSettings();

    connect(ui->numberThreshold1LineEdit, SIGNAL(textChanged(QString)), this, SLOT(setNumbersThresholds(QString)));
    connect(ui->numberThreshold2LineEdit, SIGNAL(textChanged(QString)), this, SLOT(setNumbersThresholds(QString)));
    connect(ui->mk1HightObstacleDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(computeTakeoffMinimum()));
    connect(ui->mk2HightObstacleDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(computeTakeoffMinimum()));
    connect(ui->ivppRadioButton, SIGNAL(clicked(bool)), this, SLOT(computeTakeoffMinimum()));
    connect(ui->alternateAirportCheckBox, SIGNAL(clicked(bool)), this, SLOT(computeTakeoffMinimum()));
    connect(ui->dataLoggerCheckBox, SIGNAL(clicked(bool)), this, SLOT(computeTakeoffMinimum()));
    connect(ui->withLightsCenterlineRadioButton, SIGNAL(clicked(bool)), this, SLOT(computeTakeoffMinimum()));
    connect(ui->withoutLightsCenterlineRadioButton, SIGNAL(clicked(bool)), this, SLOT(computeTakeoffMinimum()));
    connect(ui->markingCenterlineCheckBox, SIGNAL(clicked(bool)), this, SLOT(computeTakeoffMinimum()));
    connect(ui->withSideLightsRadioButton, SIGNAL(clicked(bool)), this, SLOT(computeTakeoffMinimum()));
    connect(ui->withoutSideLightsRadioButton, SIGNAL(clicked(bool)), this, SLOT(computeTakeoffMinimum()));
    connect(ui->withoutLightsCenterlineRadioButton, SIGNAL(toggled(bool)), ui->frameSideLight, SLOT(setEnabled(bool)));
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
    ui->minimumTakeoffTableWidget->horizontalHeader()->restoreState(settings.value("minimumTakeoffTable").toByteArray());
    settings.endGroup();
}

void MainDialog::writeSettings()
{
    QSettings settings;

    settings.beginGroup("geometry");
    settings.setValue("minimumTakeoffTable", ui->minimumTakeoffTableWidget->horizontalHeader()->saveState());
    settings.endGroup();
}

void MainDialog::computeTakeoffMinimum()
{
    double mk1Height = ui->mk1HightObstacleDoubleSpinBox->value();
    double mk2Height = ui->mk2HightObstacleDoubleSpinBox->value();
    double visibility_obstacle1 = 0;      // видимость при наличии препятствия
    double visibility_obstacle2 = 0;      // видимость при наличии препятствия
    double visibility = 0;               // видимость
    double visibility1 = 0;               // видимость
    double visibility2 = 0;               // видимость
    double height1 = 0;                   // высота нижней границы облаков
    double height2 = 0;                   // высота нижней границы облаков

    QObjectList checkTypeList = ui->categoryAircraftGroupBox->children();
    for (int i = 0; i < checkTypeList.size(); i++) {
        QCheckBox *obj = qobject_cast<QCheckBox*>(checkTypeList.at(i));
        if (obj && obj->isChecked()) {
            int j = 0;
            bool isDay = true;
            while (j != 2) {
                int indexCategoryAircraft = categoryAircraft.value(obj->text());

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
                height1 = mk1Height + shift;
                height1 = round(height1, 10);
                visibility_obstacle1 = 6 * height1 + 300;
                visibility_obstacle1 = round(visibility_obstacle1, 100);

                height2 = mk2Height + shift;
                height2 = round(height2, 10);
                visibility_obstacle2 = 6 * height2 + 300;
                visibility_obstacle2 = round(visibility_obstacle2, 100);

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
                                    if (isDay) {
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
                                    else if (!isDay) {
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
                                    if (isDay) {
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
                                    else if (!isDay) {
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
                                if (isDay) {
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
                                else if (!isDay) {
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
                                if (isDay) {
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
                                else if (!isDay) {
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
                                if (isDay) {
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
                                else if (!isDay) {
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

                visibility1 = visibility_obstacle1 < visibility ? visibility : visibility_obstacle1;
                QString result1 = QString("%1 x %2").arg(height1).arg(visibility1);
                visibility2 = visibility_obstacle2 < visibility ? visibility : visibility_obstacle2;
                QString result2 = QString("%1 x %2").arg(height2).arg(visibility2);

                // if night then result insert to second row
                int row = 0;
                if (!isDay)
                    row = 1;

                switch (indexCategoryAircraft) {
                    case 0:
                        ui->minimumTakeoffTableWidget->setItem(row, 2, new QTableWidgetItem(result1));
                        ui->minimumTakeoffTableWidget->setItem(row + 2, 2, new QTableWidgetItem(result2));
                        break;
                    case 1:
                        ui->minimumTakeoffTableWidget->setItem(row, 3, new QTableWidgetItem(result1));
                        ui->minimumTakeoffTableWidget->setItem(row + 2, 3, new QTableWidgetItem(result2));
                        break;
                    case 2:
                        ui->minimumTakeoffTableWidget->setItem(row, 4, new QTableWidgetItem(result1));
                        ui->minimumTakeoffTableWidget->setItem(row + 2, 4, new QTableWidgetItem(result2));
                        break;
                    case 3:
                        ui->minimumTakeoffTableWidget->setItem(row, 5, new QTableWidgetItem(result1));
                        ui->minimumTakeoffTableWidget->setItem(row + 2, 5, new QTableWidgetItem(result2));
                        break;
                    case 4:
                        ui->minimumTakeoffTableWidget->setItem(row, 6, new QTableWidgetItem(result1));
                        ui->minimumTakeoffTableWidget->setItem(row + 2, 6, new QTableWidgetItem(result2));
                        break;
                    case 5:
                        ui->minimumTakeoffTableWidget->setItem(row, 7, new QTableWidgetItem(result1));
                        ui->minimumTakeoffTableWidget->setItem(row + 2, 7, new QTableWidgetItem(result2));
                        break;
                }
                j++;
                isDay = false;
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
    ui->minimumTakeoffTableWidget->setItem(0, 0, new QTableWidgetItem(ui->numberThreshold1LineEdit->text()));
    ui->minimumTakeoffTableWidget->setItem(1, 0, new QTableWidgetItem(ui->numberThreshold1LineEdit->text()));
    ui->minimumTakeoffTableWidget->setItem(2, 0, new QTableWidgetItem(ui->numberThreshold2LineEdit->text()));
    ui->minimumTakeoffTableWidget->setItem(3, 0, new QTableWidgetItem(ui->numberThreshold2LineEdit->text()));

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
