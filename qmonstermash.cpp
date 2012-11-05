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
#include <iostream>
#include "plotdialog.h"
#include <assert.h>
#include "plotstatusbar.h"

QMonsterMash::QMonsterMash( QWidget *parent ) :
    QMainWindow( parent ),
    ui( new Ui::QMonsterMash )
{
    //Setup gui.
    ui->setupUi( this );
    ui->actMash->setEnabled( false );
    PlotStatusBar *psb = new PlotStatusBar;
    connect( this, SIGNAL( pv_changed( QString ) ) , psb, SLOT( setPv( QString ) ) );
    ui->statusBar->addWidget( psb );

    //Start EtherCAT thread
    ec = new IoThread;
    ec->start( QThread::HighestPriority );
    ec->setDigitalOutput0( false );
    ec->setDigitalOutput1( false );

    //Make damned sure that radiator output is off
    assert( ec->getDigitalOutput0() == false );

    //Set up the mash schedule widget. (Create in constructor and leave alive so settings doesent change)
    msv = new MashScheduleWidget;
    mashSchedule = msv->getMashEntries();
    mashSchedule = msv->getMashEntries();

    //Se up timer to update gui
    tmrUpdateGUI = new QTimer;
    connect( tmrUpdateGUI, SIGNAL( timeout() ), this, SLOT( updateLblPv() ) );
    connect( tmrUpdateGUI, SIGNAL( timeout() ), this, SLOT( updateLblSv() ) );
    tmrUpdateGUI->start( 200 );

    //Set up the plot widget
    ui->kpPV->setLimits( 0, PLOT_START_X, 0, PLOT_MAX_Y );
    ui->kpPV->setAntialiasing( true );

    KPlotObject *sv= new KPlotObject( Qt::green, KPlotObject::Lines, 1 );
    plotObjects.append( sv );
    KPlotObject *pv = new KPlotObject( Qt::blue, KPlotObject::Lines, 1 );
    plotObjects.append( pv );
    KPlotObject *outp = new KPlotObject( Qt::yellow, KPlotObject::Lines, 1 );
    plotObjects.append( outp );

    ui->kpPV->addPlotObjects( plotObjects );

    ui->kpPV->update();

    //Set up the minute timer that tics the plot widget and switches mash entries
    minutes = 0;
    tmrMinute = new QTimer;
    connect( tmrMinute, SIGNAL( timeout() ), this, SLOT( incrementMinutes() ) );
    tmrMinute->start( 60000 );

    //Set up pulse with modulation for output 0
    pwm = new PWMThread;
    connect( pwm, SIGNAL( statusChanged( bool ) ), ec, SLOT( setDigitalOutput0( bool ) ) );
    connect( pwm, SIGNAL( outputChanged( QString ) ), ui->lblOutput, SLOT( setText(QString) ) );
    connect( pwm, SIGNAL( outputChanged( QString ) ), psb, SLOT( setOutput( QString ) ) ); //Connect to label in statusbar

    //Set up regulator
    regSettings = new RegulatorSettings;
    reg = new Regulator;
    reg->setParameters( regSettings->getParameters() );
    connect( reg, SIGNAL( outputChanged( double ) ), pwm, SLOT( setValue( double ) ) );
    connect( regSettings, SIGNAL( parametersChanged( RegulatorSettings::reg_para_t ) ), reg, SLOT( setParameters( RegulatorSettings::reg_para_t ) ) );
    pwm->setCycleTime( regSettings->getParameters().pwmCycleTime );

    //Set up other variables
    mashRunning = false;
    pumpRunning = false;
}

QMonsterMash::~QMonsterMash()
{
    ec->setDigitalOutput0( false );
    ec->setDigitalOutput1( false );

    //Stop threads before destroying
    pwm->stop();
    pwm->wait();
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
    emit pv_changed( anin );

    //TODO delete this
    ui->lblPv->setText( anin );

    reg->setPv( ec->getAnalogInput1() );
}

//Update the lable that holds the set value
void QMonsterMash::updateLblSv()
{
    if( mashSchedule != NULL )
        ui->lblSv->setText( QString::number( mashSchedule->temp, 'f', 1 ) + QString::fromUtf8( "\u00B0" ) );
}

//This is the slot for minute timer. It tics the plot widget and switches between mash entries
void QMonsterMash::incrementMinutes()
{
    //Make shure that start mashing has been pressed
    if( !mashRunning )
        return;

    static int minutesAtSv = 0;
    static bool flagSet = false;
    if( minutes == 0 )
    {
        reg->setSv( mashSchedule->temp );
        minutesAtSv = 0;
        flagSet = false;
    }

    //Make sure that the mashschedule linked list is valid
    assert( mashSchedule != NULL );

    //Only count the minutes when pv is within the preset tolerance
    double pv = ec->getAnalogInput1();
    double tolerance = regSettings->getParameters().tolerance;
    double sv = mashSchedule->temp;

    emit sv_changed( QString::number( sv, 'g', 2 ) );

    //TODO pv > sv?
    if( pv > (sv - tolerance) )
        minutesAtSv++;

    qDebug() << minutesAtSv << ec->getAnalogInput1();


    minutes++;



    /*
    //Display progress in statusbar
    QString progressH = QString( "%1" ).arg( minutes / 60, 2, 16, QChar( '0' ) ).toUpper();
    QString progressM = QString( "%1" ).arg( minutes % 60, 2, 16, QChar( '0' ) ).toUpper();
    ui->statusBar->showMessage( tr( "Mash running: " ) + progressH + ":" + progressM );
    */

    //Expand the x axis of the plot
    if( minutes > 10 )
        ui->kpPV->setLimits( 0, minutes, 0, PLOT_MAX_Y );

    //Set a flag on the first point of every rest
    if( !flagSet && ec->getAnalogInput1() >= mashSchedule->temp )
    {
        flagSet = true;
        plotObjects[1]->addPoint( minutes, ec->getAnalogInput1(), QString( "%1" ).arg( mashSchedule->name ), 1 );
    }
    else
    {
        plotObjects[1]->addPoint( minutes, ec->getAnalogInput1() );
    }

    //Add points for set value and output
    plotObjects[0]->addPoint( minutes, mashSchedule->temp );
    plotObjects[2]->addPoint( minutes, pwm->getValue() );

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

//Tools->Hydrometer Correction pressed
void QMonsterMash::on_actHydrometerCorrection_triggered()
{
    //Destroy object after each use. No need to keep settings
    HydrometerCorrectionWidget *hcw = new HydrometerCorrectionWidget;
    hcw->show();
}

//Helper function to set gui and io in accordance with pump being on
void QMonsterMash::turn_pump_on()
{
    ec->setDigitalOutput1( true );
    ui->actMash->setEnabled( true );
    ui->actPump->setIcon( QIcon( ":/images/stopPumpIcon" ) );
    ui->actPump->setText( tr( "Stop pump" ) );

    pumpRunning = true;
}

//Helper function to set gui and io in accordance with pump being off
void QMonsterMash::turn_pump_off()
{
    turn_mash_off();

    ec->setDigitalOutput1( false );
    ui->actMash->setEnabled( false );
    ui->actPump->setIcon( QIcon( ":images/startPumpIcon" ) );
    ui->actPump->setText( tr( "Start pump" ) );

    pumpRunning = false;
}

//Helper function to set gui and regulator in accordance with mashing being on
void QMonsterMash::turn_mash_on()
{
    mashRunning = true;
    minutes = 0;


    //Reset gui
    ui->actMashSchedule->setEnabled( false );
    ui->actRegSettings->setEnabled( false );
    ui->actPlotStepResponse->setEnabled( false );
    ui->actMash->setIcon( QIcon( ":/images/stopMashIcon" ) );
    ui->actMash->setText( tr( "Stop mash" ) );
    //ui->statusBar->showMessage( tr( "Mash running: ") + "00:00" );

    //Reload the linked list with mash schedule
    mashSchedule = msv->getMashEntries();
    reg->setSv( mashSchedule->temp );

    //Reset the plot widget
    ui->kpPV->setLimits( 0, 10, 0, 80 );
    plotObjects[0]->clearPoints();
    plotObjects[1]->clearPoints();
    plotObjects[2]->clearPoints();
    ui->kpPV->update();
    plotObjects[0]->addPoint( 0, mashSchedule->temp );
    plotObjects[1]->addPoint( 0, ec->getAnalogInput1() );
    plotObjects[2]->addPoint( 0, 0 );

    //Start pulse with modulation
    pwm->start( QThread::HighPriority );

    //Start regulator
//    reg->start();
}

//Helper function to set gui and regulator in accordance with mashing being off
void QMonsterMash::turn_mash_off()
{
    mashRunning = false;

    ec->setDigitalOutput1( false );

    //Reset gui
    ui->actMashSchedule->setEnabled( true );
    ui->actRegSettings->setEnabled( true );
    ui->actPlotStepResponse->setEnabled( true );
    ui->actMash->setIcon( QIcon( ":/images/startMashIcon" ) );
    ui->actMash->setText( tr( "Start mash" ) );
    //ui->statusBar->showMessage( "" );

    //Stop pulse with modulation
    pwm->stop();
    pwm->wait();

    //stop regulator
    reg->stop();
}

//Toolbar -> start pump button pressed
void QMonsterMash::on_actPump_triggered()
{
    if( !pumpRunning )
        turn_pump_on();
    else
        turn_pump_off();
}

//Toolbar -> start mash button pressed
void QMonsterMash::on_actMash_triggered()
{
    if( !mashRunning )
        turn_mash_on();
    else
        turn_mash_off();
}

//Edit->Regulator settings pressed
void QMonsterMash::on_actRegSettings_triggered()
{
        regSettings->show();
}

//Tools->Plot step response
void QMonsterMash::on_actPlotStepResponse_triggered()
{
    PlotDialog *pd = new PlotDialog( 0, ec );
    connect( pd, SIGNAL( startManual( double ) ), pwm, SLOT( startManual( double ) ) );
    connect( pd, SIGNAL( stopManual() ), pwm, SLOT( stop() ) );
    pd->show();
}
