#include "maindialog.h"
#include "ui_maindialog.h"
#include <QtWidgets/QMessageBox>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtCore/QSettings>
#include <QtGui/QStandardItemModel>
#include "minimum.h"
#include <QDebug>

MainDialog::MainDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainDialog)
{
    ui->setupUi(this);

    model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels(QStringList()
                                     << tr("Category aircraft")
                                     << tr("Minimum takeoff")
                                     << tr("ILS")
                                     << tr("PSP")
                                     << tr("RMS-PSP")
                                     << tr("RSP+OSP")
                                     << tr("RSP")
                                     << tr("OSP"));


    minimum = new Minimum(model);

    if (!connectDatabase()) {
        qWarning() << "Error connect to database";
        exit(1);
    }
    getListAirfields();
    readSettings();

    model->setItem(0, 0, new QStandardItem("H"));
    model->setItem(1, 0, new QStandardItem("A"));
    model->setItem(2, 0, new QStandardItem("B"));
    model->setItem(3, 0, new QStandardItem("C"));
    model->setItem(4, 0, new QStandardItem("D"));

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

bool MainDialog::connectDatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setHostName("localhost");
    db.setDatabaseName(QString("Driver={Microsoft Access Driver (*.mdb)};DBQ=%1").arg("D:/projects/Qt/ObstacleCreator/db/ADB.mdb"));
    if (!db.open()) {
        qDebug() << db.databaseName();
        qWarning() << db.lastError().text();
        return false;
    }
    return true;
}

void MainDialog::getListAirfields()
{
    QSqlQuery query("SELECT id_aero, NAMEI, NAMEI2 FROM AERODROMI ORDER BY NAMEI, NAMEI2");
    while(query.next()) {
    }
}

void MainDialog::getListObstaclesByAirfield(int index)
{
    QSqlQuery query;

    query.prepare("SELECT MAX(Habs) FROM PREPARPT WHERE CODA = ?");
    if (!query.exec()) {
        qWarning() << query.lastError().text();
        QMessageBox::warning(this, tr("Warning"), tr("Failed to get list of obstacles!"));
        return;
    }
    query.first();
    minimum->setHeightObstacle(query.value(0).toFloat());
    fillTable();
}

void MainDialog::setOvi()
{
    minimum->setOvi(qobject_cast<QRadioButton*>(sender())->isChecked());
    fillTable();
}

void MainDialog::setTimeOfDay()
{
    minimum->setTimeOfDay(qobject_cast<QRadioButton*>(sender())->isChecked());
    fillTable();
}

void MainDialog::setLights()
{
    minimum->setLights(qobject_cast<QRadioButton*>(sender())->isChecked());
    fillTable();
}

void MainDialog::fillTable()
{
    minimum->computeMinimum();
}


