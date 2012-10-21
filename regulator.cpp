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

#include "regulator.h"


Regulator::Regulator( QObject */*parent*/ )
{
    cycleTimer = new QTimer;
    connect( cycleTimer, SIGNAL( timeout() ), this, SLOT( updatePI() ) );
    IState = 0;
}

//Main regulator function. Calculate output.
void Regulator::updatePI()
{
    double error = sv - pv;

    double pTerm = error * ProportionalGain;

    IState += error;

    if( IState >= iMax )  //Make sure that IState stays in between IMin and IMax
        IState = iMax;
    else if( IState <= iMin )
        IState = iMin;

    double iTerm = Integral * IState;

    output = pTerm + iTerm;

    if( output > 100.0f )
        output = 100.0f;
    else if( output < 0.0f )
        output = 0.0f;


    emit outputChanged( output );
}

//Start regulator
void Regulator::start()
{
    IntegralState = 0;
    updatePI();
    cycleTimer->start( cycleTime );
}

//Stop regulator
void Regulator::stop()
{
    cycleTimer->stop();
    output = 0;

    emit outputChanged( 0.0f );
}

//Save new set value
void Regulator::setSv(double newValue)
{
        if( sv > newValue ) //Clear IState if Sv is lowered
                IState = 0;

        sv = newValue;
}

//Save new processvalue
void Regulator::setPv(double newValue)
{
        pv = newValue;
}

//Return current outuput
double Regulator::getOutput()
{
        return output;
}

//Setup new parameters
void Regulator::setParameters( RegulatorSettings::reg_para_t newPara )
{
        ProportionalGain = newPara.P;
        Integral = newPara.I;
        cycleTime = newPara.cycleTime;
        iMax = newPara.Imax;
        iMin = newPara.Imin;
}
