#include "mashscheduledialog.h"
#include "ui_mashscheduledialog.h"
#include <QDebug>
#include "doublespinboxdelegate.h"

MashScheduleDialog::MashScheduleDialog( QWidget *parent, MashSchedule *ms ) :
    QDialog( parent ), ui( new Ui::MashScheduleDialog )
{
    ui->setupUi(this);

    twMashScheduleModel = ms;
    ui->twMashSchedule->setModel( twMashScheduleModel );
    DoubleSpinBoxDelegate *dspnd = new DoubleSpinBoxDelegate;
    ui->twMashSchedule->setItemDelegateForColumn( 0, dspnd );
    ui->twMashSchedule->horizontalHeader()->setResizeMode( QHeaderView::Stretch );
}

MashScheduleDialog::~MashScheduleDialog()
{
    delete ui;
}

void MashScheduleDialog::on_buttNew_clicked()
{
    twMashScheduleModel->appendRow();
}

void MashScheduleDialog::on_buttDel_clicked()
{
    twMashScheduleModel->removeRow( ui->twMashSchedule->currentIndex().row(), QModelIndex() );
}
