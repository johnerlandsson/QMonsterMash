#include "hydrometercorrectionwidget.h"
#include "ui_hydrometercorrectionwidget.h"
#include <math.h>
#include <QDebug>

HydrometerCorrectionWidget::HydrometerCorrectionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HydrometerCorrectionWidget)
{
    ui->setupUi(this);
}

HydrometerCorrectionWidget::~HydrometerCorrectionWidget()
{
    delete ui;
}

void HydrometerCorrectionWidget::recalculate()
{
    double t1c = (double)ui->spnSgTemp->value();
    double t2c = (double)ui->spnCalTemp->value();

    double t1 = 1.8f * t1c + 32;
    double t2 = 1.8f * t2c + 32;
    qDebug() << t1;

    double SG1 = 1.00130346 - (1.34722124E-4 * t1) + (2.04052596E-6 * pow( t1, 2 )) - (2.32820948E-9 * pow( t1, 3 ));
    double SG2 = 1.00130346 - (1.34722124E-4 * t2) + (2.04052596E-6 * pow( t2, 2 )) - (2.32820948E-9 * pow( t2, 3 ));

    double SGcorr = ui->dspnSg->value() * SG1 / SG2;

    ui->lblResult->setText( QString::number( SGcorr, 'f', 3 ) );
}

void HydrometerCorrectionWidget::on_spnCalTemp_valueChanged( int /*arg1*/ )
{
    recalculate();
}

void HydrometerCorrectionWidget::on_spnSgTemp_valueChanged( int /*arg1*/ )
{
    recalculate();
}

void HydrometerCorrectionWidget::on_dspnSg_valueChanged( double /*arg1*/ )
{
    recalculate();
}
