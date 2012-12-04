#include "boiltimerdialog.h"
#include "ui_boiltimerdialog.h"
#include <QDebug>
#include <QMessageBox>
#include "completinglineeditdelegate.h"

BoilTimerDialog::BoilTimerDialog( QWidget *parent ) : QDialog( parent ), ui( new Ui::BoilTimerDialog )
{
    ui->setupUi( this );

    secTimer = new QTimer;
    connect( secTimer, SIGNAL( timeout() ), this, SLOT( countdown() ) );

    boilSchedule = new BoilSchedule;
    ui->twBoilSchedule->setModel( boilSchedule );

    ui->twBoilSchedule->horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    CompletingLineEditDelegate *nameDelegate = new CompletingLineEditDelegate( ui->twBoilSchedule, ":/text/boilwords.txt" );
    ui->twBoilSchedule->setItemDelegateForColumn( 2, nameDelegate );

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
}

void BoilTimerDialog::on_buttAdd_clicked()
{
    boilSchedule->appendRow( 0, 1, "New" );
}
