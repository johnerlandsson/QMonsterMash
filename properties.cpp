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

#include "properties.h"
#include "ui_properties.h"
#include <QMessageBox>

Properties::Properties(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Properties)
{
    ui->setupUi(this);

    settings.beginGroup( "QMonsterMash" );
    ui->spnPWMTime->setValue( settings.value( "PWMCycleTime" ).toInt() );
    ui->chkPlot->setChecked( settings.value( "plot" ).toBool() );
    settings.endGroup();
}

Properties::~Properties()
{
    delete ui;
}

//Returns the cycle time for pulse width modulation
int Properties::getPWMCycleTime()
{
    return ui->spnPWMTime->value();
}

//Returns the value of the plot temperature check box
bool Properties::getPlot()
{
    return ui->chkPlot->isChecked();
}

//Settings saved
void Properties::on_buttSave_clicked()
{
    settings.beginGroup( "QMonsterMash" );
    settings.setValue( "PWMCycleTime", ui->spnPWMTime->value() );
    settings.setValue( "plot", ui->chkPlot->isChecked() );
    settings.endGroup();

    QMessageBox::information( this, "Saved", "Settings saved" );
}
