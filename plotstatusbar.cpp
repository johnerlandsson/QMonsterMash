#include "plotstatusbar.h"
#include "ui_plotstatusbar.h"
#include <QDebug>

PlotStatusBar::PlotStatusBar( QWidget *parent ) :
    QWidget( parent ),
    ui(new Ui::PlotStatusBar)
{
    ui->setupUi(this);

    ui->lblBlue->setStyleSheet( "QLabel{ background-color: blue }" );
    ui->lblGreen->setStyleSheet( "QLabel{ background-color: #00FF00 }" );
    ui->lblYellow->setStyleSheet( "QLabel{ background-color: yellow }" );

    timeProgress = 0;
    ui->pbMashProgress->setValue( timeProgress );
}

void PlotStatusBar::incrementProgress()
{
    timeProgress++;
    ui->pbMashProgress->setValue( timeProgress );
}

PlotStatusBar::~PlotStatusBar()
{
    delete ui;
}

void PlotStatusBar::setPv( QString newValue )
{
    ui->lblPv->setText( newValue );
}

void PlotStatusBar::setSv( QString newValue )
{
    ui->lblSv->setText( newValue );
}

void PlotStatusBar::setOutput( QString newValue )
{
    ui->lblOutput->setText( newValue );
}

void PlotStatusBar::setTotalTime( int time )
{
    ui->pbMashProgress->setMaximum( time );
    timeProgress = 0;
}
