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

#ifndef REGULATOR_H
#define REGULATOR_H

#include <QObject>
#include <QTimer>
#include "regulatorsettings.h"

class Regulator : public QObject
{
        Q_OBJECT
    public:
        explicit Regulator( QObject *parent = 0 );
        void start();
        void stop();
        double getOutput();

    private:
        QTimer *cycleTimer;
        double output;
        double IState;
        double ProportionalGain;
        double Integral;
        double IntegralState;
        unsigned int cycleTime;
        double sv, pv, iMax, iMin;
        
    signals:
        void outputChanged( double newValue );
        void svChanged( QString newValue );
        
    public slots:
        void updatePI();
        void setSv( double newValue );
        void setPv( double newValue );
        void setParameters( RegulatorSettings::reg_para_t newPara );
        
};

#endif // REGULATOR_H
