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
