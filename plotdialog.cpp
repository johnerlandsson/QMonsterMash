#include "plotdialog.h"
#include "ui_plotdialog.h"
#include <QDebug>
#include <QFileDialog>

PlotDialog::PlotDialog( QWidget *parent, IoThread *io ) :
        QDialog(parent),
        ui(new Ui::PlotDialog),
        ec( io )
{
    ui->setupUi(this);

    sampleTimer = new QTimer;
    connect( sampleTimer, SIGNAL( timeout() ), this, SLOT( takeSample() ) );

    filePath.clear();
}

PlotDialog::~PlotDialog()
{
    delete ui;

    sampleCounter = 0;
}
void PlotDialog::on_buttStart_clicked()
{
        sampleCounter = 0;

        if( ec == NULL )
                return;

        file.setFileName( filePath );

        if( !file.open( QFile::WriteOnly ) )
                return;

        sampleTimer->start( ui->spnSampleTime->value() );
        emit startManual( ui->dspnOutput->value() );
        ec->setDigitalOutput1( true );
}

void PlotDialog::on_buttBrowse_clicked()
{
        filePath = QFileDialog::getSaveFileName( this, "Save plot", QDir::homePath(), "Comma separated value (*.csv)" );
        if( filePath.section( ".", 1, 1 ) != "csv" )
                filePath.append( ".csv" );
}

void PlotDialog::on_buttStop_clicked()
{
    sampleTimer->stop();

    file.close();

    emit stopManual();
}

void PlotDialog::takeSample()
{
        QTextStream stream( &file );
        stream << sampleCounter << "," << ec->getAnalogInput1() << "\n";

    sampleCounter++;
}
