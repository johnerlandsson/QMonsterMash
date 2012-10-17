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

void PWMThread::stop()
{
    running = false;
}

void PWMThread::setValue( double newValue )
{
    mutex.lock();
    value = newValue;
    mutex.unlock();
}

void PWMThread::setCycleTime(unsigned int newCycleTime)
{
    mutex.lock();
    cycleTime = newCycleTime;
    mutex.unlock();
}
