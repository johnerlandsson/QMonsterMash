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

#include "hydrometercorrectionwidget.h"
#include "ui_hydrometercorrectionwidget.h"
#include <math.h>
#include <QDebug>

HydrometerCorrectionWidget::HydrometerCorrectionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HydrometerCorrectionWidget)
{
    ui->setupUi(this);
}

HydrometerCorrectionWidget::~HydrometerCorrectionWidget()
{
    delete ui;
}

void HydrometerCorrectionWidget::recalculate()
{
    double t1c = (double)ui->spnSgTemp->value();
    double t2c = (double)ui->spnCalTemp->value();

    double t1 = 1.8f * t1c + 32;
    double t2 = 1.8f * t2c + 32;

    double SG1 = 1.00130346 - (1.34722124E-4 * t1) + (2.04052596E-6 * pow( t1, 2 )) - (2.32820948E-9 * pow( t1, 3 ));
    double SG2 = 1.00130346 - (1.34722124E-4 * t2) + (2.04052596E-6 * pow( t2, 2 )) - (2.32820948E-9 * pow( t2, 3 ));

    double SGcorr = ui->dspnSg->value() * SG1 / SG2;

    ui->lblResult->setText( QString::number( SGcorr, 'f', 3 ) );
}

void HydrometerCorrectionWidget::on_spnCalTemp_valueChanged( int /*arg1*/ )
{
    recalculate();
}

void HydrometerCorrectionWidget::on_spnSgTemp_valueChanged( int /*arg1*/ )
{
    recalculate();
}

void HydrometerCorrectionWidget::on_dspnSg_valueChanged( double /*arg1*/ )
{
    recalculate();
}
