#ifndef QMONSTERMASH_H
#define QMONSTERMASH_H

#include <QMainWindow>
#include <QTimer>
#include "iothread.h"
#include <kplotobject.h>
#include "mashschedulewidget.h"
#include "pwmthread.h"

namespace Ui {
    class QMonsterMash;
}

class QMonsterMash : public QMainWindow
{
        Q_OBJECT
        
    public:
        explicit QMonsterMash(QWidget *parent = 0);
        ~QMonsterMash();
        
    private:
        Ui::QMonsterMash *ui;

        IoThread *ec;
        QTimer *tmrUpdateGUI;
        QTimer *tmrMinute;

        unsigned int minutes;

        KPlotObject *kpoPV;

        MashScheduleWidget *msv;

        bool mashRunning;
        bool pumpRunning;

        MashScheduleWidget::mashEntry_t *mashSchedule;

        PWMThread *pwm;

    private slots:
        void updateLblPv();
        void updateLblSv();
        void incrementMinutes();
        void on_actExit_triggered();
        void on_actBoilTimer_triggered();
        void on_actMashSchedule_triggered();
        void on_buttStart_clicked();
        void on_buttStop_clicked();
        void on_actHydrometerCorrection_triggered();
        void on_buttStartPump_clicked();
        void on_buttStopPump_clicked();
};

#endif // QMONSTERMASH_H
