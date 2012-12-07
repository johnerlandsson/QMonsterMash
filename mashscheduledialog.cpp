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

#include "mashscheduledialog.h"
#include "ui_mashscheduledialog.h"
#include <QDebug>
#include "doublespinboxdelegate.h"
#include "wordfilestringlistmodel.h"
#include "completinglineeditdelegate.h"
#include "spinboxdelegate.h"
#include "doublespinboxdelegate.h"

MashScheduleDialog::MashScheduleDialog( QWidget *parent, MashSchedule *ms ) :
    QDialog( parent ), ui( new Ui::MashScheduleDialog )
{
    ui->setupUi(this);

    twMashScheduleModel = ms;
    ui->twMashSchedule->setModel( twMashScheduleModel );
    DoubleSpinBoxDelegate *dspnd = new DoubleSpinBoxDelegate;
    ui->twMashSchedule->setItemDelegateForColumn( 0, dspnd );
    ui->twMashSchedule->horizontalHeader()->setResizeMode( QHeaderView::Stretch );

    WordfileStringListModel *nameDelegateModel = new WordfileStringListModel( NULL, ":text/mashwords.txt" );
    CompletingLineEditDelegate *nameDelegate = new CompletingLineEditDelegate( twMashScheduleModel, nameDelegateModel );
    ui->twMashSchedule->setItemDelegateForColumn( MashSchedule::Name, nameDelegate );

    SpinBoxDelegate *timeDelegate = new SpinBoxDelegate;
    ui->twMashSchedule->setItemDelegateForColumn( MashSchedule::Time, timeDelegate );
    DoubleSpinBoxDelegate *tempDelegate = new DoubleSpinBoxDelegate;
    tempDelegate->setMaxValue( 80.0f );
    tempDelegate->setMinValue( 10 );
    ui->twMashSchedule->setItemDelegateForColumn( MashSchedule::Temperature, tempDelegate );
}

MashScheduleDialog::~MashScheduleDialog()
{
    delete ui;
}

void MashScheduleDialog::on_buttNew_clicked()
{
    twMashScheduleModel->appendRow();
}

void MashScheduleDialog::on_buttDel_clicked()
{
    int row = ui->twMashSchedule->currentIndex().row();

    twMashScheduleModel->removeRow( row );
}
