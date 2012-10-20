#include "regulatorsettings.h"
#include "ui_regulatorsettings.h"
#include <QMessageBox>
#include <QDebug>

RegulatorSettings::RegulatorSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegulatorSettings)
{
        ui->setupUi(this);

        settings.beginGroup( "QMonsterMash" );
        ui->dspnKp->setValue( settings.value( "kP" ).toDouble() );
        ui->dspnI->setValue( settings.value( "I" ).toDouble() );
        ui->dspnIMax->setValue( settings.value( "IMax" ).toDouble() );
        ui->dspnIMin->setValue( settings.value( "IMin" ).toDouble() );
        ui->spnCycleTime->setValue( settings.value( "cycleTime" ).toInt() );
        settings.endGroup();
        qDebug() << ui->spnCycleTime->value();
}

RegulatorSettings::~RegulatorSettings()
{
        delete ui;
}

//Save button clicked
void RegulatorSettings::on_buttSave_clicked()
{
        if( ui->dspnIMax->value() < ui->dspnIMin->value() )
        {
                QMessageBox::critical( this, "Error", "IMax has to be greater than IMin.\nSettings not saved" );
                return;
        }

        settings.beginGroup( "QMonsterMash" );
        settings.setValue( "kP", ui->dspnKp->value() );
        settings.setValue( "I", ui->dspnI->value() );
        settings.setValue( "IMax", ui->dspnIMax->value() );
        settings.setValue( "IMin", ui->dspnIMin->value() );
        settings.setValue( "cycleTime", ui->spnCycleTime->value() );
        settings.endGroup();

        qDebug() << ui->dspnKp->value();
        qDebug() << settings.value( "QMonsterMash/kP" );

        QMessageBox::information( this, "Done", "Regulator settings saved..." );
}

RegulatorSettings::reg_para_t RegulatorSettings::getParameters()
{
        reg_para_t ret;
        ret.cycleTime = ui->spnCycleTime->value();
        ret.I = ui->dspnI->value();
        ret.P = ui->dspnKp->value();
        ret.Imax = ui->dspnIMax->value();
        ret.Imin = ui->dspnIMin->value();

        return ret;
}
