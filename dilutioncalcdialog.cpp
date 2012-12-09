#include "dilutioncalcdialog.h"
#include "ui_dilutioncalcdialog.h"

DilutionCalcDialog::DilutionCalcDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DilutionCalcDialog)
{
    ui->setupUi(this);

    connect( ui->dspnCurrGravity, SIGNAL( valueChanged(double) ), this, SLOT( calculate() ) );
    connect( ui->dspnCurrVolume, SIGNAL( valueChanged(double) ), this, SLOT( calculate() ) );
    connect( ui->dspnDesGravity, SIGNAL( valueChanged(double) ), this, SLOT( calculate() ) );

    calculate();
}

DilutionCalcDialog::~DilutionCalcDialog()
{
    delete ui;
}

void DilutionCalcDialog::calculate()
{
    double gravity = ui->dspnCurrGravity->value() - 1;
    double volume = ui->dspnCurrVolume->value();
    double finalGravity = ui->dspnDesGravity->value() - 1;

    double addVol = ((volume * gravity) / finalGravity) - volume;

    ui->lblResult->setText( QString::number( addVol, 'f', 2 ) + " Liters of water" );
}
