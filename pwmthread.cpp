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

#include "pwmthread.h"
#include <sys/time.h>
#include <QDebug>

PWMThread::PWMThread( QObject */*parent*/ )
{
    running = false;
    value = 0;
    ec = NULL;
}

// Thread run method. All the good stuff happens here
void PWMThread::run()
{
    running = true;

    emit statusChanged( false );

    double tmpValue = 0;
    int tmpCycleTime = 0;

    timeval start, stop;

    while( running )
    {
        gettimeofday( &start, NULL );

        mutex.lock();
        tmpValue = value;
        tmpCycleTime = cycleTime;
        mutex.unlock();

        if( tmpValue >= 100 )
        {
            emit statusChanged( true );
            continue;
        }
        else if( tmpValue <= 0 )
        {
            emit statusChanged( false );
            continue;
        }

        int onTime = (tmpCycleTime * (tmpValue * 0.01));

        //Turn on
        emit statusChanged( true );

        //Use time.h for precition delay
        while( true )
        {
            gettimeofday( &stop, NULL );
            long time = ((stop.tv_sec * 1000000 ) + stop.tv_usec ) - ((start.tv_sec * 1000000) + start.tv_usec);
            if( time >= onTime * 1000 )
                break;
        }

        //Turn off
        emit statusChanged( false );
        while( true )
        {
            gettimeofday( &stop, NULL );
            long time = ((stop.tv_sec * 1000000 ) + stop.tv_usec ) - ((start.tv_sec * 1000000) + start.tv_usec);
            if( time >= tmpCycleTime * 1000 )
                break;
        }
    }
}

//Stop thread
void PWMThread::stop()
{
    running = false;

    emit outputChanged( QString::number( 0.0f, 'g', 2 ) + "%" );
    emit statusChanged( false );
}

//Set new output value Public
void PWMThread::setValue( double newValue )
{
    mutex.lock();
    value = newValue;
    mutex.unlock();

    emit outputChanged( QString::number( newValue, 'g', 2 ) + "%" );
}

void PWMThread::setCycleTime(unsigned int newCycleTime)
{
    mutex.lock();
    cycleTime = newCycleTime;
    mutex.unlock();
}

void PWMThread::startManual( double manValue )
{
    value = manValue;

    start( QThread::HighPriority );

    emit outputChanged( QString::number( manValue, 'g', 2 ) + "%" );
}

//Public method to return current output value
double PWMThread::getValue()
{
    mutex.lock();
    double ret = value;
    mutex.unlock();

    return ret;
}
