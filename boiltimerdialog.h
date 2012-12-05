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

#ifndef BOILTIMERDIALOG_H
#define BOILTIMERDIALOG_H

#include <QDialog>
#include "boilschedule.h"
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

        void on_buttAdd_clicked();

private:
        Ui::BoilTimerDialog *ui;
        BoilSchedule *boilSchedule;
        QTimer *secTimer;
        int secoundsRemaining;
        void setRemainingLabel( int secounds );
        void setNextLabel( int secounds );
};

#endif // BOILTIMERDIALOG_H
