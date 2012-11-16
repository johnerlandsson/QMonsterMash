#ifndef MASHSCHEDULE_H
#define MASHSCHEDULE_H

#include <QList>
#include <QStringList>

class MashSchedule
{
public:
    MashSchedule();
    double getCurrentTemp();
    int getCurrentTime();
    QString getCurrentName();
    bool next();
    int count();
    void reset();
    double getTemp( int index );
    bool setTemp( int index, double value );
    int getTime( int index );
    bool setTime( int index, int value );
    QString getName( int index );
    bool setName( int index, QString value );
    void addEntry( double temp, int time, QString name );
    void clear();
    bool isAtEnd();

private:
    QList<double> tempSchedule;
    QList<int> timeSchedule;
    QStringList nameSchedule;
    int currentIndex;
};

#endif // MASHSCHEDULE_H
