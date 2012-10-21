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

#include "qmonstermash.h"
#include "ui_qmonstermash.h"
#include <QDebug>
#include "hydrometercorrectionwidget.h"
#include "boiltimerwidget.h"

QMonsterMash::QMonsterMash(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QMonsterMash)
{
    ui->setupUi(this);

    //Start EtherCAT thread
    ec = new IoThread;
    ec->start( QThread::HighestPriority );
    ec->setDigitalOutput0( false );
    ec->setDigitalOutput1( false );

    //Set up the mash schedule widget. (Create in constructor and leave alive so settings doesent change)
    msv = new MashScheduleWidget;
    mashSchedule = msv->getMashEntries();
    mashSchedule = msv->getMashEntries();

    //Se up timer to update gui
    tmrUpdateGUI = new QTimer;
    connect( tmrUpdateGUI, SIGNAL( timeout() ), this, SLOT( updateLblPv() ) );
    connect( tmrUpdateGUI, SIGNAL( timeout() ), this, SLOT( updateLblSv() ) );
    connect( tmrUpdateGUI, SIGNAL( timeout() ), this, SLOT( updateLblOutput() ) );
    tmrUpdateGUI->start( 200 );

    //Set up the plot widget
    ui->kpPV->setLimits( 0, PLOT_MAX_X, 0, PLOT_MAX_Y );
    ui->kpPV->setAntialiasing( true );

    kpoPV = new KPlotObject( Qt::green, KPlotObject::Lines, 1 );
    ui->kpPV->addPlotObject( kpoPV );

    //Set up the minute timer that tics the plot widget and switches mash entries
    minutes = 0;
    tmrMinute = new QTimer;
    connect( tmrMinute, SIGNAL( timeout() ), this, SLOT( incrementMinutes() ) );
    tmrMinute->start( 60000 );

    //Set up properties widget
    prop = new Properties;

    //Set up pulse with modulation for output 0
    pwm = new PWMThread;
    pwm->setCycleTime( prop->getPWMCycleTime() );
    connect( pwm, SIGNAL( statusChanged( bool ) ), ec, SLOT( setDigitalOutput0( bool ) ) );

    //Set up regulator
    regSettings = new RegulatorSettings;
    reg = new Regulator;
    reg->setParameters( regSettings->getParameters() );
    connect( reg, SIGNAL( outputChanged( double ) ), pwm, SLOT( setValue( double ) ) );
    connect( regSettings, SIGNAL( parametersChanged( RegulatorSettings::reg_para_t ) ), reg, SLOT( setParameters( RegulatorSettings::reg_para_t ) ) );

    //Set up other variables
    mashRunning = false;
    pumpRunning = false;
}

QMonsterMash::~QMonsterMash()
{
    //Stop thread before destroying
    ec->stop();
    ec->wait();
    delete ui;
}

//File->Exit pressed
void QMonsterMash::on_actExit_triggered()
{
    exit( EXIT_SUCCESS );
}

//Update the lable that holds the process value
void QMonsterMash::updateLblPv()
{
    QString anin = QString::number( ec->getAnalogInput1(), 'f', 1 ) + QString::fromUtf8( "\u00B0" );
    ui->lblPv->setText( anin );

    reg->setPv( ec->getAnalogInput1() );
}

//Update the lable that holds the set value
void QMonsterMash::updateLblSv()
{
    if( mashSchedule != NULL )
        ui->lblSv->setText( QString::number( mashSchedule->temp, 'f', 1 ) + QString::fromUtf8( "\u00B0" ) );
}

//Update tha label that holds the regulator output
void QMonsterMash::updateLblOutput()
{
        ui->lblOutput->setText( QString::number( reg->getOutput(), 'f', 1 ) + "%" );
}

//This is the slot for minute timer. It tics the plot widget and switches between mash entries
void QMonsterMash::incrementMinutes()
{
    static int minutesAtSv = 0;
    static bool flagSet = false;
    if( minutes == 0 )
    {
        reg->setSv( mashSchedule->temp );
        minutesAtSv = 0;
        flagSet = false;
    }

    //Make shure that start mashing has been pressed
    if( !mashRunning )
        return;

    //Make sure that the mashschedule linked list is valid
    if( mashSchedule == NULL )
        throw "Bad pointer from MashScheduleWidget";


    //Only count the minutes when pv>=sv
    if( ec->getAnalogInput1() >= mashSchedule->temp )
        minutesAtSv++;

    minutes++;

    //Expand the x axis of the plot
    if( minutes > 10 )
        ui->kpPV->setLimits( 0, minutes, 0, PLOT_MAX_Y );

    //Set a flag on the first point of every rest
    if( !flagSet && ec->getAnalogInput1() >= mashSchedule->temp )
    {
        flagSet = true;
        kpoPV->addPoint( minutes, ec->getAnalogInput1(), QString( "%1" ).arg( mashSchedule->name ), 1 );
    }
    else
    {
        kpoPV->addPoint( minutes, ec->getAnalogInput1() );
    }
    ui->kpPV->update();

    //Stop switching mash entries when at the last object of the mash schedule linked list
    if( mashSchedule->next == NULL )
        return;
    else if( minutesAtSv >= mashSchedule->time ) //This part switches mash entries
    {
        minutesAtSv = 0;
        flagSet = false;

        MashScheduleWidget::mashEntry_t *tmp = mashSchedule->next;
        delete mashSchedule;
        mashSchedule = tmp;

        reg->setSv( mashSchedule->temp );
    }
}

//Tools->Boil timer pressed
void QMonsterMash::on_actBoilTimer_triggered()
{
    BoilTimerWidget *btw = new BoilTimerWidget;
    btw->show();
}

//File->Mash Schedule pressed
void QMonsterMash::on_actMashSchedule_triggered()
{
    msv->show();
}

//Start mash button pressed
void QMonsterMash::on_buttStart_clicked()
{
    mashRunning = true;
    minutes = 0;
    pwm->setCycleTime( prop->getPWMCycleTime() );

    //Reset gui
    ui->buttStart->setEnabled( false );
    ui->buttStop->setEnabled( true );
    ui->actMashSchedule->setEnabled( false );
    ui->actRegSettings->setEnabled( false );

    //Reload the linked list with mash schedule
    mashSchedule = msv->getMashEntries();
    reg->setSv( mashSchedule->temp );

    //Reset the plot widget
    ui->kpPV->setLimits( 0, 10, 0, 80 );
    kpoPV->clearPoints();
    ui->kpPV->update();
    kpoPV->addPoint( 0, ec->getAnalogInput1() );

    //Start pulse with modulation
    pwm->start( QThread::HighPriority );

    //Start regulator
    reg->start();
}

//Stop mash button pressed
void QMonsterMash::on_buttStop_clicked()
{
    mashRunning = false;

    ec->setDigitalOutput1( false );

    //Reset gui
    ui->buttStart->setEnabled( true );
    ui->buttStop->setEnabled( false );
    ui->actMashSchedule->setEnabled( true );
    ui->actRegSettings->setEnabled( true );

    //Stop pulse with modulation
    pwm->stop();
    pwm->wait();

    //stop regulator
    reg->stop();
}

//Tools->Hydrometer Correction pressed
void QMonsterMash::on_actHydrometerCorrection_triggered()
{
    //Destroy object after each use. No need to keep settings
    HydrometerCorrectionWidget *hcw = new HydrometerCorrectionWidget;
    hcw->show();
}

//Start pump button pressed
void QMonsterMash::on_buttStartPump_clicked()
{
    pumpRunning = true;
    ec->setDigitalOutput1( true );

    //Reset gui
    ui->buttStart->setEnabled( true );
    ui->buttStartPump->setEnabled( false );
    ui->buttStopPump->setEnabled( true );
}

//Stop pump button pressed
void QMonsterMash::on_buttStopPump_clicked()
{
    pumpRunning = false;
    ui->buttStartPump->setEnabled( true );
    ui->buttStopPump->setEnabled( false );

    on_buttStop_clicked();
}

//Edit->Regulator settings pressed
void QMonsterMash::on_actRegSettings_triggered()
{
        regSettings->show();
}

//Edit->Properties pressed
void QMonsterMash::on_actProperties_triggered()
{
    prop->show();
}
