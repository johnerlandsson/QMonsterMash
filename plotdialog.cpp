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

#include "plotdialog.h"
#include "ui_plotdialog.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <assert.h>

PlotDialog::PlotDialog( QWidget *parent, IoThread *io ) :
        QDialog(parent),
        ui(new Ui::PlotDialog),
        ec( io )
{
        ui->setupUi(this);

        assert( ec != NULL );

        sampleTimer = new QTimer;
        connect( sampleTimer, SIGNAL( timeout() ), this, SLOT( takeSample() ) );

        filePath.clear();
}

PlotDialog::~PlotDialog()
{
    delete ui;

    sampleCounter = 0;
}

// Start button pressed
void PlotDialog::on_buttStart_clicked()
{
        sampleCounter = 0;

        if( filePath == "" )
        {
                QMessageBox::information( this, "Filename", "No file selected" );
                return;
        }

        file.setFileName( filePath );

        if( !file.open( QFile::WriteOnly ) )
        {
                QMessageBox::critical( this, "Error", QString( "Could not open file \"%1\" for writing." ).arg( filePath ) );
                return;
        }

        sampleTimer->start( ui->spnSampleTime->value() );
        emit startManual( ui->dspnOutput->value() );
        ec->setDigitalOutput1( true );

        ui->buttStop->setEnabled( true );
        ui->buttStart->setEnabled( false );
}

//Select file to save
void PlotDialog::on_buttBrowse_clicked()
{
        filePath = QFileDialog::getSaveFileName( this, "Save plot", QDir::homePath(), "Comma separated value (*.csv)" );
        if( filePath.section( ".", 1, 1 ) != "csv" )
                filePath.append( ".csv" );
}

//Stop button pressed
void PlotDialog::on_buttStop_clicked()
{
        sampleTimer->stop();
        file.close();

        emit stopManual();

        ui->buttStart->setEnabled( true );
        ui->buttStop->setEnabled( false );
}

//This is the method that saves a sample at the given time
void PlotDialog::takeSample()
{
        QTextStream stream( &file );
        stream << sampleCounter << "," << ec->getAnalogInput1() << "\n";

    sampleCounter++;
}
