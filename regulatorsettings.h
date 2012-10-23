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

#ifndef REGULATORSETTINGS_H
#define REGULATORSETTINGS_H

#include <QWidget>
#include <QSettings>

namespace Ui {
    class RegulatorSettings;
}

class RegulatorSettings : public QWidget
{
        Q_OBJECT

public:
        typedef struct
        {
                double I;
                double P;
                double Imax;
                double Imin;
                int cycleTime;
                int pwmCycleTime;
        }reg_para_t;

        reg_para_t getParameters();

        explicit RegulatorSettings( QWidget *parent = 0 );
        ~RegulatorSettings();

signals:
        void parametersChanged( RegulatorSettings::reg_para_t newParameters );

private slots:
        void on_buttSave_clicked();

private:
        Ui::RegulatorSettings *ui;
        QSettings settings;
};

#endif // REGULATORSETTINGS_H
