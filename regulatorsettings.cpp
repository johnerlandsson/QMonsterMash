/*  Copyright (C) 2012 John Erlandsson

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "regulatorsettings.h"
#include "ui_regulatorsettings.h"
#include <QMessageBox>
#include <QDebug>

RegulatorSettings::RegulatorSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegulatorSettings)
{
        ui->setupUi(this);

        //Read settings from QSettings object and setup GUI
        settings.beginGroup( "QMonsterMash" );
        ui->dspnKp->setValue( settings.value( "kP" ).toDouble() );
        ui->dspnI->setValue( settings.value( "I" ).toDouble() );
        ui->dspnIMax->setValue( settings.value( "IMax" ).toDouble() );
        ui->dspnIMin->setValue( settings.value( "IMin" ).toDouble() );
        ui->spnCycleTime->setValue( settings.value( "cycleTime" ).toInt() );
        settings.endGroup();
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

        //Values from GUI
        double kp = ui->dspnKp->value();
        double i = ui->dspnI->value();
        double imax = ui->dspnIMax->value();
        double imin = ui->dspnIMin->value();
        int cycletime = ui->spnCycleTime->value();

        //Save to QSettings object
        settings.beginGroup( "QMonsterMash" );
        settings.setValue( "kP", kp );
        settings.setValue( "I", i );
        settings.setValue( "IMax", imax );
        settings.setValue( "IMin", imin );
        settings.setValue( "cycleTime", cycletime );
        settings.endGroup();

        //Send new parameters as signal
        reg_para_t para;
        para.cycleTime = cycletime;
        para.I = i;
        para.P = kp;
        para.Imax = imax;
        para.Imin = imin;

        emit parametersChanged( para );
        QMessageBox::information( this, "Done", "Regulator settings saved..." );
}

//Slot to return current settings.
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
