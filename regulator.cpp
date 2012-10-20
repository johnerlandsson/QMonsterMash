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
#include <QDebug>


Regulator::Regulator( QObject *parent, double kP, double I, unsigned int cycleTimeMs ) :
    QObject( parent ), ProportionalGain( kP ), Integral( I ), cycleTime( cycleTimeMs )
{
    cycleTimer = new QTimer;
    connect( cycleTimer, SIGNAL( timeout() ), this, SLOT( updatePI() ) );
}

void Regulator::updatePI()
{
    double error = sv - pv;
    qDebug() << "Error: " << sv << " - " << pv << " = " << error;

    double pTerm = error * ProportionalGain;
    qDebug() << "pTerm: " << error << " * " << ProportionalGain << " = " << pTerm;

    IState += error;
    if( IState >= 100.0f )
        IState = 100.0f;
    else if( IState <= 0.0f )
        IState = 0.0f;
    qDebug() << "iState: " << IState;

    double iTerm = Integral * IState;
    qDebug() << "iTerm: " << Integral << " * " << IState << " = " << iTerm;

    output = pTerm + iTerm;

    if( output > 100.0f )
        output = 100.0f;
    else if( output < 0.0f )
        output = 0.0f;
    emit outputChanged( output );

    qDebug() << "Output: " << pTerm << " + " << iTerm << " = " << output;
    qDebug() << "";
}

void Regulator::start()
{
    IntegralState = 0;
    cycleTimer->start( cycleTime );
}

void Regulator::stop()
{
    cycleTimer->stop();
    output = 0;

    emit outputChanged( 0.0f );
}

void Regulator::setSv(double newValue)
{
        if( sv > newValue )
                IState = 0;

        sv = newValue;
}

void Regulator::setPv(double newValue)
{
        pv = newValue;
}

double Regulator::getOutput()
{
        return output;
}
