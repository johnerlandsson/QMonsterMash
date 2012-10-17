#include "mashschedulewidget.h"
#include "ui_mashschedulewidget.h"
#include <string.h>

MashScheduleWidget::MashScheduleWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MashScheduleWidget)
{
    ui->setupUi(this);
    mashEntries = NULL;
}

MashScheduleWidget::~MashScheduleWidget()
{
    delete ui;
}

//Slot for enable acid rest check button
void MashScheduleWidget::on_chkAcidEnable_toggled( bool checked )
{
    ui->dspnAcidTemp->setEnabled( checked );
    ui->spnAcidMin->setEnabled( checked );
}

//Slot for enable protein rest check button
void MashScheduleWidget::on_chkProteinEnable_toggled( bool checked )
{
    ui->dspnProteinTemp->setEnabled( checked );
    ui->spnProteinMin->setEnabled( checked );
}

//Slot for enable Saccrification rest2 check button
void MashScheduleWidget::on_chkSac2Enable_toggled(bool checked)
{
    ui->dspnSacTemp2->setEnabled( checked );
    ui->spnSacTime2->setEnabled( checked );
}

//Slot for enable Saccrification rest3 check button
void MashScheduleWidget::on_chkSac3Enable_toggled(bool checked)
{
    ui->dspnSacTemp3->setEnabled( checked );
    ui->spnSacTime3->setEnabled( checked );
}

//This function resets the mash entries and returns their pointer
MashScheduleWidget::mashEntry_t *MashScheduleWidget::getMashEntries()
{
    //Empty the linked list
    while( mashEntries != NULL )
    {
        mashEntry_t *tmp = mashEntries->next;
        delete mashEntries;
        mashEntries = tmp;
    }

    mashEntries = new mashEntry_t;
    mashEntries->next = NULL;
    mashEntry_t *head = mashEntries;

    //Add acid rest to mash schedule if enabled
    if( ui->chkAcidEnable->isChecked() )
    {
        mashEntries->temp = ui->dspnAcidTemp->value();
        mashEntries->time = ui->spnAcidMin->value();
        strcpy( mashEntries->name, "Acid" );

        mashEntries->next = new mashEntry_t;
        mashEntries = mashEntries->next;
        mashEntries->next = NULL;
    }
    //Add protein rest to mash schedule if enabled
    if( ui->chkProteinEnable->isChecked() )
    {
        mashEntries->temp = ui->dspnProteinTemp->value();
        mashEntries->time = ui->spnProteinMin->value();
        strcpy( mashEntries->name, "Protein" );

        mashEntries->next = new mashEntry_t;
        mashEntries = mashEntries->next;
        mashEntries->next = NULL;
    }
    //Add saccrification rest1 to mash schedule if enabled
    if( ui->chkSac1Enable->isChecked() )
    {
        mashEntries->temp = ui->dspnSacTemp1->value();
        mashEntries->time = ui->spnSacTime1->value();
        strcpy( mashEntries->name, "Saccrification1" );

        mashEntries->next = new mashEntry_t;
        mashEntries = mashEntries->next;
        mashEntries->next = NULL;
    }
    //Add saccrification rest2 to mash schedule if enabled
    if( ui->chkSac2Enable->isChecked() )
    {
        mashEntries->temp = ui->dspnSacTemp2->value();
        mashEntries->time = ui->spnSacTime2->value();
        strcpy( mashEntries->name, "Saccrification2" );

        mashEntries->next = new mashEntry_t;
        mashEntries = mashEntries->next;
        mashEntries->next = NULL;
    }
    //Add saccrification rest3 to mash schedule if enabled
    if( ui->chkSac3Enable->isChecked() )
    {
        mashEntries->temp = ui->dspnSacTemp3->value();
        mashEntries->time = ui->spnSacTime3->value();
        strcpy( mashEntries->name, "Saccrification3" );

        mashEntries->next = new mashEntry_t;
        mashEntries = mashEntries->next;
        mashEntries->next = NULL;
    }

    //Add sparge to mash schedule
    mashEntries->temp = ui->dspnSpargeTemp->value();
    mashEntries->time = -1;
    strcpy( mashEntries->name, "Sparge" );
    mashEntries->next = NULL;



    mashEntries = head;
    return mashEntries;
}
