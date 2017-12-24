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

    readSettings();

    qDebug() << round(45, 10);
    qDebug() << round(157, 100);


    connect(ui->ivppRadioButton, SIGNAL(clicked(bool)), this, SLOT(computeTakeoffMinimum()));
    connect(ui->alternateAirportCheckBox, SIGNAL(clicked(bool)), this, SLOT(computeTakeoffMinimum()));
    connect(ui->dataLoggerCheckBox, SIGNAL(clicked(bool)), this, SLOT(computeTakeoffMinimum()));
    connect(ui->withLightsCenterlineRadioButton, SIGNAL(clicked(bool)), this, SLOT(computeTakeoffMinimum()));
    connect(ui->categoryAircraftComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(computeTakeoffMinimum()));
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

    if (mk1Height != 0 || mk2Height != 0) {
        heightObstacle = qMax(mk1Height, mk2Height);

        double shift = 0;
        switch (ui->categoryAircraftComboBox->currentIndex()) {
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
                    switch (ui->categoryAircraftComboBox->currentIndex()) {
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
                            switch (ui->categoryAircraftComboBox->currentIndex()) {
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
                            switch (ui->categoryAircraftComboBox->currentIndex()) {
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
                            switch (ui->categoryAircraftComboBox->currentIndex()) {
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
                            switch (ui->categoryAircraftComboBox->currentIndex()) {
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
                        switch (ui->categoryAircraftComboBox->currentIndex()) {
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
                        switch (ui->categoryAircraftComboBox->currentIndex()) {
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
                switch (ui->categoryAircraftComboBox->currentIndex()) {
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
                        switch (ui->categoryAircraftComboBox->currentIndex()) {
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
                        switch (ui->categoryAircraftComboBox->currentIndex()) {
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
                        switch (ui->categoryAircraftComboBox->currentIndex()) {
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
                        switch (ui->categoryAircraftComboBox->currentIndex()) {
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

    qDebug() << "Lvid = " << visibility;
    qDebug() << "Hngo = " << height;
    qDebug() << "Lvid_obst = " << visibility_obstacle;

    ui->visibilityLineEdit->setText(QString::number(visibility));
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
