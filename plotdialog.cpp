#include "plotdialog.h"
#include "ui_plotdialog.h"
#include <QDebug>

PlotDialog::PlotDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlotDialog)
{
    ui->setupUi(this);

    sampleTimer = new QTimer;
    connect( sampleTimer, SIGNAL( timeout() ), this, SLOT( takeSample() ) );
}

PlotDialog::~PlotDialog()
{
    delete ui;

    sampleCounter = 0;
}
void PlotDialog::on_buttStart_clicked()
{
    sampleTimer->start( ui->spnSampleTime->value() );
    emit startManual( ui->dspnOutput->value() );
}

void PlotDialog::on_buttBrowse_clicked()
{

}

void PlotDialog::on_buttStop_clicked()
{
    sampleTimer->stop();
    emit stopManual();
}

void PlotDialog::takeSample()
{
    qDebug() << sampleCounter << ",";

    sampleCounter++;
}
