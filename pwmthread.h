#ifndef PWMTHREAD_H
#define PWMTHREAD_H

#include <QThread>
#include <QMutex>
#include "iothread.h"

class PWMThread : public QThread
{
    Q_OBJECT

    public:
        PWMThread( QObject *parent = 0 );
        void run();
        void stop();

        void setValue( double newValue );
        void setCycleTime( unsigned int newCycleTime );

    signals:
        void statusChanged( bool newStatus );

    private:
        unsigned int cycleTime;
        bool running;
        QMutex mutex;
        double value;
        IoThread *ec;

};

#endif // PWMTHREAD_H
