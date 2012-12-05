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

#include "boiltimerdialog.h"
#include "ui_boiltimerdialog.h"
#include <QDebug>
#include <QMessageBox>
#include "completinglineeditdelegate.h"
#include "wordfilestringlistmodel.h"

BoilTimerDialog::BoilTimerDialog( QWidget *parent ) : QDialog( parent ), ui( new Ui::BoilTimerDialog )
{
    ui->setupUi( this );

    secTimer = new QTimer;
    connect( secTimer, SIGNAL( timeout() ), this, SLOT( countdown() ) );

    boilSchedule = new BoilSchedule( ui->twBoilSchedule );
    ui->twBoilSchedule->setModel( boilSchedule );

    ui->twBoilSchedule->horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    WordfileStringListModel *nameDelegateModel = new WordfileStringListModel( NULL, ":text/boilwords.txt" );
    CompletingLineEditDelegate *nameDelegate = new CompletingLineEditDelegate( ui->twBoilSchedule, nameDelegateModel );
    ui->twBoilSchedule->setItemDelegateForColumn( BoilSchedule::Name, nameDelegate );

    setRemainingLabel( ui->spnTotalTime->value() * 60 );
    setNextLabel( 0 );
}

BoilTimerDialog::~BoilTimerDialog()
{
    delete ui;
}

void BoilTimerDialog::on_buttStart_clicked()
{
    ui->spnTotalTime->setEnabled( false );
    ui->twBoilSchedule->setEditTriggers( QAbstractItemView::NoEditTriggers );

    secoundsRemaining = ui->spnTotalTime->value() * 60;
    secTimer->start( 1000 );
}

void BoilTimerDialog::on_buttStop_clicked()
{
    secTimer->stop();
    ui->spnTotalTime->setEnabled( true );
    ui->twBoilSchedule->setEditTriggers( QAbstractItemView::DoubleClicked |
                                         QAbstractItemView::AnyKeyPressed |
                                         QAbstractItemView::SelectedClicked );
}

void BoilTimerDialog::countdown()
{
    if( secoundsRemaining <= 0 )
    {
        on_buttStop_clicked();
        return;
    }

    setRemainingLabel( secoundsRemaining - 1 );

    int nextSec = 0;
    for( int i = 0; i < boilSchedule->rowCount(); i++ )
    {
        int addSec = boilSchedule->getTime( i ) * 60;

        if( secoundsRemaining == addSec )
        {
            QMessageBox *info = new QMessageBox( QMessageBox::Information, tr( "Addition" ), tr( "Please add " ) + boilSchedule->getName( i ) + tr( " now." ) );
            info->show();
        }

        if( addSec < secoundsRemaining && addSec > nextSec  )
            nextSec = addSec;
    }

    secoundsRemaining--;

    setNextLabel( secoundsRemaining - nextSec );
}

void BoilTimerDialog::setRemainingLabel( int secounds )
{
    int minutes = secounds / 60;
    secounds %= 60;

    QString dispMin, dispSec;
    dispMin.sprintf( "%02d", minutes );
    dispSec.sprintf( "%02d", secounds );

    ui->lblTotal->setText( dispMin + ":" + dispSec );
}

void BoilTimerDialog::setNextLabel( int secounds )
{
    int minutes = secounds / 60;
    secounds %= 60;

    QString dispMin, dispSec;
    dispMin.sprintf( "%02d", minutes );
    dispSec.sprintf( "%02d", secounds );

    ui->lblNext->setText( dispMin + ":" + dispSec );
}



void BoilTimerDialog::on_spnTotalTime_valueChanged(int arg1)
{
    setRemainingLabel( arg1 * 60 );
}

void BoilTimerDialog::on_buttDelete_clicked()
{
    int row = ui->twBoilSchedule->currentIndex().row();

    boilSchedule->removeRow( row );
}

void BoilTimerDialog::on_buttAdd_clicked()
{
    boilSchedule->appendRow( 0, 1, "New" );
}
