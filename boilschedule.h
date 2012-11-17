#ifndef BOILSCHEDULE_H
#define BOILSCHEDULE_H

#include <QStringList>

class BoilSchedule
{
    public:
        BoilSchedule();
        QStringList nameSchedule;
        QList<int> timeSchedule;
        int count();
        void appendRow( int time, QString name );
        bool setTime( int index, int value );
        bool setName( int index, QString value );
        int getTime( int index );
        QString getName( int index );
};

#endif // BOILSCHEDULE_H
