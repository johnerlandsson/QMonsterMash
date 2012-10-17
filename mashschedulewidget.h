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

#ifndef MASHSCHEDULEWIDGET_H
#define MASHSCHEDULEWIDGET_H

#include <QWidget>

namespace Ui {
    class MashScheduleWidget;
}


class MashScheduleWidget : public QWidget
{
        Q_OBJECT

        
    public:
        struct mashEntry
        {
                double temp;
                int time;
                char name[17];

                struct mashEntry *next;
        };
        typedef struct mashEntry mashEntry_t;

    public:
        explicit MashScheduleWidget( QWidget *parent = 0 );
        ~MashScheduleWidget();
        mashEntry_t *getMashEntries();
        
    private slots:
        void on_chkAcidEnable_toggled(bool checked);
        void on_chkProteinEnable_toggled(bool checked);
        void on_chkSac2Enable_toggled(bool checked);
        void on_chkSac3Enable_toggled(bool checked);

    private:
        Ui::MashScheduleWidget *ui;
        mashEntry_t *mashEntries;
};

#endif // MASHSCHEDULEWIDGET_H
