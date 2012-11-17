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
#include "boiltimerdialog.h"
#include <iostream>
#include "plotdialog.h"
#include <assert.h>
#include "plotstatusbar.h"
#include "mashscheduledialog.h"
#include <QMessageBox>

QMonsterMash::QMonsterMash( QWidget *parent ) :
    QMainWindow( parent ),
    ui( new Ui::QMonsterMash )
{
    //Setup gui.
    ui->setupUi( this );
    ui->actMash->setEnabled( false );

    PlotStatusBar *psb = new PlotStatusBar;
    connect( this, SIGNAL( pv_changed( QString ) ) , psb, SLOT( setPv( QString ) ) );
    ui->statusBar->insertPermanentWidget( 0, psb, 1 );


    //Start EtherCAT thread
    ec = new IoThread;
    ec->start( QThread::HighestPriority );
    ec->setDigitalOutput0( false );
    ec->setDigitalOutput1( false );


    //Se up timer to update gui
    tmrUpdateGUI = new QTimer;
    connect( tmrUpdateGUI, SIGNAL( timeout() ), this, SLOT( updateLblPv() ) );
    connect( this, SIGNAL( sv_changed( QString ) ), psb, SLOT( setSv( QString ) ) );
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
    connect( pwm, SIGNAL( outputChanged( QString ) ), psb, SLOT( setOutput( QString ) ) ); //Connect to label in statusbar
    pwm->setValue( 0 );


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

    //Make damned sure that radiator output is off
    assert( ec->getDigitalOutput0() == false );
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

    reg->setPv( ec->getAnalogInput1() );
}

//This is the slot for minute timer. It tics the plot widget and switches between mash entries
void QMonsterMash::incrementMinutes()
{
    double pv = ec->getAnalogInput1();
    double sv = mashSchedule.getCurrentTemp();

    //Make shure that start mashing has been pressed
    if( !mashRunning )
        return;

    static int minutesAtSv = 0;
    static bool flagSet = false;
    if( minutes == 0 )
    {
        reg->setSv( sv );
        minutesAtSv = 0;
        flagSet = false;
    }

    //Only count the minutes when pv is within the preset tolerance
    double tolerance = regSettings->getParameters().tolerance;

    //TODO pv > sv?
    if( pv > (sv - tolerance) )
        minutesAtSv++;


    minutes++;


    //Expand the x axis of the plot
    if( minutes > 10 )
        ui->kpPV->setLimits( 0, minutes, 0, PLOT_MAX_Y );

    //Set a flag on the first point of every rest
    if( !flagSet && pv >= sv )
    {
        flagSet = true;
        plotObjects[1]->addPoint( minutes, pv, QString( "%1" ).arg( mashSchedule.getCurrentName() ), 1 );
    }
    else
    {
        plotObjects[1]->addPoint( minutes, pv );
    }

    //Add points for set value and output
    plotObjects[0]->addPoint( minutes, sv );
    plotObjects[2]->addPoint( minutes, pwm->getValue() );

    ui->kpPV->update();

    if( minutesAtSv >= mashSchedule.getCurrentTime() ) //This part switches mash entries
    {
        if( mashSchedule.isAtEnd() )
        {
            turn_mash_off();
            QMessageBox::information( this, tr( "Done" ), tr( "Mashing complete..." ) );
            return;
        }

        minutesAtSv = 0;
        flagSet = false;

        mashSchedule.next();
        reg->setSv( mashSchedule.getCurrentTemp() );

        emit sv_changed( QString::number( mashSchedule.getCurrentTemp(), 'f', 1 ) + QString::fromUtf8( "\u00B0" ) );
    }
}

//Tools->Boil timer pressed
void QMonsterMash::on_actBoilTimer_triggered()
{
    BoilTimerDialog *btd = new BoilTimerDialog;
    btd->show();
}

//File->Mash Schedule pressed
void QMonsterMash::on_actMashSchedule_triggered()
{
    MashScheduleDialog *msd = new MashScheduleDialog( this );
    msd->exec();
    mashSchedule = msd->getSchedule();
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

    reg->setSv( mashSchedule.getCurrentTemp() );
    emit sv_changed( QString::number( mashSchedule.getCurrentTemp(), 'f', 1 ) + QString::fromUtf8( "\u00B0" ) );

    //Reset the plot widget
    ui->kpPV->setLimits( 0, 10, 0, 80 );
    plotObjects[0]->clearPoints();
    plotObjects[1]->clearPoints();
    plotObjects[2]->clearPoints();
    ui->kpPV->update();
    plotObjects[0]->addPoint( 0, 0.0f );
    plotObjects[1]->addPoint( 0, ec->getAnalogInput1() );
    plotObjects[2]->addPoint( 0, 0 );

    //Start pulse with modulation
    pwm->start( QThread::HighPriority );

    //Start regulator
    reg->start();

    //Reset index counter to 0
    mashSchedule.reset();
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

    //stop regulator
    reg->stop();

    //Stop pulse with modulation
    pwm->stop();
    pwm->wait();

    assert( !ec->getDigitalOutput0() );
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
