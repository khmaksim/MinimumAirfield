#include "dialog.h"
#include "ui_dialog.h"
#include <QtWidgets/QMessageBox>
#include "minimums.h"
#include <QDebug>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    minimums = new Minimums();

    readSettings();

    connect(ui->categoryAircraftComboBox, SIGNAL(activated(QString)), this, SLOT(computeMinimums()));
    connect(ui->withLightsRadioButton, SIGNAL(toggled(bool)), this, SLOT(computeMinimums()));
    connect(ui->withoutLightsRadioButton, SIGNAL(toggled(bool)), this, SLOT(computeMinimums()));
    connect(ui->withoutLightsRadioButton, SIGNAL(toggled(bool)), ui->oviGroupBox, SLOT(setEnabled(bool)));
    connect(ui->withOviRadioButton, SIGNAL(toggled(bool)), this, SLOT(computeMinimums()));
    connect(ui->withoutOviRadioButton, SIGNAL(toggled(bool)), this, SLOT(computeMinimums()));
    connect(ui->dayRadioButton, SIGNAL(toggled(bool)), this, SLOT(computeMinimums()));
    connect(ui->nightRadioButton, SIGNAL(toggled(bool)), this, SLOT(computeMinimums()));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::readSettings()
{
    ui->categoryAircraftComboBox->addItem(tr("A"));
    ui->categoryAircraftComboBox->setItemData(ui->categoryAircraftComboBox->count() - 1,
                                              tr("Ан-2, Ан-14, Ан-28, Л-410, Ил-14, Як-18, Як-50, Як-52, Су26, все типы вертолетов"), Qt::ToolTipRole);
    ui->categoryAircraftComboBox->addItem(tr("B"));
    ui->categoryAircraftComboBox->setItemData(ui->categoryAircraftComboBox->count() - 1,
                                              tr("Л-29, Л-39, Як-40, Як-42, Ан-8, Ан-24, Ан-26, Ан-30, Ан-72, Ан-74, Ил-114"), Qt::ToolTipRole);
    ui->categoryAircraftComboBox->addItem(tr("C"));
    ui->categoryAircraftComboBox->setItemData(ui->categoryAircraftComboBox->count() - 1,
                                              tr("Ан-32, Ан-50, Ту-134, Ил-76, Ил-78, БЕ-12, Ту-16, МиГ-17, Су-17"), Qt::ToolTipRole);
    ui->categoryAircraftComboBox->addItem(tr("D"));
    ui->categoryAircraftComboBox->setItemData(ui->categoryAircraftComboBox->count() - 1,
                                              tr("Ил-18, Ил-20, Ил-38, Ил-62, Ил-86, Ту-134А, Ту-142, Ту-154, Ту-95, Ан-12, Ан-22, Ан-124, МиГ-21, МиГ-29, 3М, М-4, Су-25, Су-27, Ту-204, Ил-96"), Qt::ToolTipRole);
    ui->categoryAircraftComboBox->addItem(tr("E"));
    ui->categoryAircraftComboBox->setItemData(ui->categoryAircraftComboBox->count() - 1,
                                              tr("Су-15, Су-17, Су-24, Як-28, Як-38, МиГ-23, МиГ-25, МиГ-27, МиГ-31, Ту-22, Ту-95МС"), Qt::ToolTipRole);
}

void Dialog::computeMinimums()
{
    if (ui->withLightsRadioButton->isChecked())
        ui->result->setText(QString::number(minimums->getMinimumsWithLights(ui->categoryAircraftComboBox->currentText())));
    else {
        if (ui->withoutOviRadioButton->isChecked())
            if (ui->dayRadioButton->isChecked())
                ui->result->setText(QString::number(minimums->getMinimumsWithoutLightsWithoutOviDay(ui->categoryAircraftComboBox->currentText())));
            else
                ui->result->setText(QString::number(minimums->getMinimumsWithoutLightsWithoutOviNight(ui->categoryAircraftComboBox->currentText())));
        else
            if (ui->nightRadioButton->isChecked())
                ui->result->setText(QString::number(minimums->getMinimumsWithoutLightsWithOviDay(ui->categoryAircraftComboBox->currentText())));
            else
                ui->result->setText(QString::number(minimums->getMinimumsWithoutLightsWithOviNight(ui->categoryAircraftComboBox->currentText())));
    }
}
