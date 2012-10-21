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

#include "boiltimerwidget.h"
#include "ui_boiltimerwidget.h"
#include <QMessageBox>
#include <QFile>
#include <phonon/MediaObject>

BoilTimerWidget::BoilTimerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BoilTimerWidget)
{
    ui->setupUi(this);

    running = false;

    //Set up the tablemodel
    tblAdditionsModel = new QStandardItemModel( 5, 2 );

    QStringList header;
    header << "Time" << "Name";
    tblAdditionsModel->setHorizontalHeaderLabels( header );

    //TODO add standard additions in a proper way.
    QStandardItem *item = new QStandardItem( "Bittering hops" );
    tblAdditionsModel->setItem( 0, 1, item );
    item = new QStandardItem;
    item->setData( 60, Qt::DisplayRole );
    tblAdditionsModel->setItem( 0, 0, item );

    item = new QStandardItem( "Flavor hops" );
    tblAdditionsModel->setItem( 1, 1, item );
    item = new QStandardItem;
    item->setData( 15, Qt::DisplayRole );
    tblAdditionsModel->setItem( 1, 0, item );

    item = new QStandardItem( "Irish moss" );
    tblAdditionsModel->setItem( 2, 1, item );
    item = new QStandardItem;
    item->setData( 15, Qt::DisplayRole );
    tblAdditionsModel->setItem( 2, 0, item );

    item = new QStandardItem( "Yeast nutrient" );
    tblAdditionsModel->setItem( 3, 1, item );
    item = new QStandardItem;
    item->setData( 5, Qt::DisplayRole );
    tblAdditionsModel->setItem( 3, 0, item );

    item = new QStandardItem( "Aroma hops" );
    tblAdditionsModel->setItem( 4, 1, item );
    item = new QStandardItem;
    item->setData( 6, Qt::DisplayRole );
    tblAdditionsModel->setItem( 4, 0, item );


    ui->tblAdditions->setModel( tblAdditionsModel );
    ui->tblAdditions->resizeColumnsToContents();
    ui->tblAdditions->resizeRowsToContents();

    //Update timer
    connect( &tmrUpdateLblTime, SIGNAL( timeout() ), this, SLOT( updateLblTime() ) );

    sortValidateModel();
}

BoilTimerWidget::~BoilTimerWidget()
{
    delete ui;
}

// Start boiltimer pressed
void BoilTimerWidget::on_buttStart_clicked()
{
    if( !running )      //When timer is stopped
    {
        sortValidateModel();
        if( tblAdditionsModel->rowCount() < 1  )
        {
            QMessageBox::critical( this, "Error", "No additions set" );
            return;
        }

        ui->buttStart->setText( "Stop" );
        ui->buttAdd->setEnabled( false );
        ui->buttRemove->setEnabled( false );
        ui->tblAdditions->setEditTriggers( QTableView::NoEditTriggers );

        running = true;
        currentAddition = 0;

        tmrUpdateLblTime.start( 200 );

        timerStarted.start();
    }
    else        //When timer is running
    {
        ui->buttStart->setText( "Start" );
        ui->buttAdd->setEnabled( true );
        ui->buttRemove->setEnabled( true );
        ui->tblAdditions->setEditTriggers( QTableView::DoubleClicked | QTableView::AnyKeyPressed );

        running = false;
        tmrUpdateLblTime.stop();
    }
}

// Add addition pressed
void BoilTimerWidget::on_buttAdd_clicked()
{
    tblAdditionsModel->insertRow( tblAdditionsModel->rowCount() );
    ui->tblAdditions->resizeColumnsToContents();
    ui->tblAdditions->resizeRowsToContents();
}

//Timeout slot for update timer
void BoilTimerWidget::updateLblTime()
{
    int elapsedSec = timerStarted.elapsed() / 1000;
    int totalSec = ui->spnTime->value() * 60;

    int remainingSec = totalSec - elapsedSec;

    //Update total countdown label
    QString dispMin;
    dispMin.sprintf( "%02d", remainingSec / 60);
    QString dispSec;
    dispSec.sprintf( "%02d", remainingSec % 60 );

    ui->lblTime->setText( dispMin + ":" + dispSec );

    if( remainingSec <= 0 )
    {
        QMessageBox::information( this, "Done", "Flameout!" );
        ui->lblAdditionTime->setText( "-" );
        on_buttStart_clicked(); //Stop countdown
        return;
    }

    //Fetch current addition
    QStandardItem *time = tblAdditionsModel->item( currentAddition, 0 );
    QStandardItem *name = tblAdditionsModel->item( currentAddition, 1 );

    if( time == NULL ) //No more additions in list
    {
        ui->lblAdditionTime->setText( "-" );
        return;
    }

    //Calculate countdown for current additon
    int additionSec = time->data( Qt::DisplayRole ).toInt() * 60;
    int additionRemSec = remainingSec - additionSec;

    if( additionRemSec <= 0 ) //Current addition done
    {
        currentAddition++;

        //Play alarm sound
        Phonon::MediaObject *doneSound = Phonon::createPlayer( Phonon::MusicCategory, Phonon::MediaSource( ":/sounds/done" ) );
        doneSound->play();

        QString msgName = name == NULL ? "Unnamed addition" : name->data( Qt::DisplayRole ).toString();
        QApplication::beep();
        QString msg = "Please add " + msgName + " now.";

        QMessageBox::information( this, "Addition", msg );
        return;
    }

    //Update addition countdown label
    QString dispAdditionMin, dispAdditionSec;
    dispAdditionMin.sprintf( "%02d", additionRemSec / 60 );
    dispAdditionSec.sprintf( "%02d", additionRemSec % 60 );

    ui->lblAdditionTime->setText( dispAdditionMin + ":" + dispAdditionSec );


}

//Sort table by time value and remove invalid entries
void BoilTimerWidget::sortValidateModel()
{
    if( tblAdditionsModel->rowCount() < 1 )
        return;

    for( int row = 0; row < tblAdditionsModel->rowCount(); row++ )
    {
        QStandardItem *min = tblAdditionsModel->item( row, 0 );

        if( min == NULL )
        {
            tblAdditionsModel->removeRow( row );
            continue;
        }

        int value = min->data( Qt::DisplayRole ).toInt();
        if( value < 1 || value > ui->spnTime->value() )
            tblAdditionsModel->removeRow( row );
    }

    tblAdditionsModel->sort( 0, Qt::DescendingOrder );
}

//Remove addition pressed
void BoilTimerWidget::on_buttRemove_clicked()
{
    int row = ui->tblAdditions->currentIndex().row();

    tblAdditionsModel->removeRow( row );
}
