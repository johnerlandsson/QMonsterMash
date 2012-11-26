#ifndef BOILTIMERDIALOG_H
#define BOILTIMERDIALOG_H

#include <QDialog>
#include "boilscheduletablemodel.h"
#include <QTimer>

namespace Ui {
    class BoilTimerDialog;
}

class BoilTimerDialog : public QDialog
{
        Q_OBJECT
        
    public:
        explicit BoilTimerDialog( QWidget *parent = 0 );
        ~BoilTimerDialog();
        
    private slots:
        void on_buttStart_clicked();
        void countdown();

        void on_buttStop_clicked();

        void on_spnTotalTime_valueChanged(int arg1);

        void on_buttDelete_clicked();

private:
        Ui::BoilTimerDialog *ui;
        BoilSchedule boilSchedule;
        BoilScheduleTableModel *twBoilScheduleModel;
        QTimer *secTimer;
        int secoundsRemaining;
        void setRemainingLabel( int secounds );
        void setNextLabel( int secounds );
};

#endif // BOILTIMERDIALOG_H
