#include "regulatorsettings.h"
#include "ui_regulatorsettings.h"
#include <QMessageBox>
#include <QSettings>

RegulatorSettings::RegulatorSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegulatorSettings)
{
        ui->setupUi(this);

        QSettings settings;

        ui->dspnKp->setValue( settings.value( "QMonsterMash/kP" ).toDouble() );
        ui->dspnI->setValue( settings.value( "QMonsterMash/I" ).toDouble() );
        ui->dspnIMax->setValue( settings.value( "QMonsterMash/IMax" ).toDouble() );
        ui->dspnIMin->setValue( settings.value( "QMonsterMash/IMin" ).toDouble() );
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

        QSettings settings;

        settings.setValue( "QMonsterMash/kP", ui->dspnKp->value() );
        settings.setValue( "QMonsterMash/I", ui->dspnI->value() );
        settings.setValue( "QMonsterMash/IMax", ui->dspnIMax->value() );
        settings.setValue( "QMonsterMash/IMin", ui->dspnIMin->value() );

        QMessageBox::information( this, "Done", "Regulator settings saved..." );
}
