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

#ifndef QMONSTERMASH_H
#define QMONSTERMASH_H

#include <QMainWindow>
#include <QTimer>
#include "iothread.h"
#include <kplotobject.h>
#include "mashschedulewidget.h"
#include "pwmthread.h"
#include "regulator.h"

#define PLOT_MAX_Y 100
#define PLOT_MAX_X 10

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
        Regulator *reg;

    private slots:
        void updateLblPv();
        void updateLblSv();
        void updateLblOutput();
        void incrementMinutes();
        void on_actExit_triggered();
        void on_actBoilTimer_triggered();
        void on_actMashSchedule_triggered();
        void on_buttStart_clicked();
        void on_buttStop_clicked();
        void on_actHydrometerCorrection_triggered();
        void on_buttStartPump_clicked();
        void on_buttStopPump_clicked();
        void on_actRegSettings_triggered();
};

#endif // QMONSTERMASH_H
