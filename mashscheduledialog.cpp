#include "mashscheduledialog.h"
#include "ui_mashscheduledialog.h"
#include <QDebug>
#include "doublespinboxdelegate.h"

MashScheduleDialog::MashScheduleDialog( QWidget *parent, MashSchedule ms ) :
    QDialog( parent ), ui( new Ui::MashScheduleDialog )
{
    ui->setupUi(this);

    twMashScheduleModel = new MashScheduleTableModel( 0 );
    twMashScheduleModel->setSchedule( ms );
    ui->twMashSchedule->setModel( twMashScheduleModel );
    DoubleSpinBoxDelegate *dspnd = new DoubleSpinBoxDelegate;
    ui->twMashSchedule->setItemDelegateForColumn( 0, dspnd );
    connect( twMashScheduleModel, SIGNAL( dataChanged(QModelIndex,QModelIndex ) ), this, SLOT( resetSchedule() ) );
}

MashScheduleDialog::~MashScheduleDialog()
{
    delete ui;
}

MashSchedule MashScheduleDialog::getSchedule()
{
    return twMashScheduleModel->getSchedule();
}

void MashScheduleDialog::resetSchedule()
{
    ret = twMashScheduleModel->getSchedule();
    emit newSchedule( ret );
}

void MashScheduleDialog::on_buttNew_clicked()
{
    twMashScheduleModel->insertRow( twMashScheduleModel->rowCount(), QModelIndex() );
}
