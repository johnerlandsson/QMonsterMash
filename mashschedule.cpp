#include "mashschedule.h"
#include <QDebug>

MashSchedule::MashSchedule()
{
    tempSchedule.append( 35.0f );    //Default acid rest
    timeSchedule.append( 15 );
    nameSchedule << "Acid rest";

    tempSchedule.append( 50.0f );    //Default protein rest
    timeSchedule.append( 15 );
    nameSchedule << "Protein rest";

    tempSchedule.append( 68.0f );    //Default saccrification rest
    timeSchedule.append( 60 );
    nameSchedule << "Saccrification rest";

    tempSchedule.append( 76.0f );    //Default sparge
    timeSchedule.append( -1 );
    nameSchedule << "Sparge";

    currentIndex = 0;
}

void MashSchedule::reset()
{
    currentIndex = 0;
}

bool MashSchedule::isAtEnd()
{
    return currentIndex >= (count() - 1);
}

bool MashSchedule::next()
{
    bool ret = false;

    if( currentIndex < (count() - 1) )
    {
        currentIndex++;
        ret = true;
    }

    return ret;
}

int MashSchedule::count()
{
    if( timeSchedule.count() != tempSchedule.count() || tempSchedule.count() != nameSchedule.count() )
    {
        throw "Mash schedule lists dont match";
    }

    return tempSchedule.count();
}

double MashSchedule::getTemp( int index )
{
    if( index >= count() )
        return -1;

    return tempSchedule[index];
}

double MashSchedule::getCurrentTemp()
{
    return tempSchedule[currentIndex];
}

int MashSchedule::getTime( int index )
{
    if( index >= count() )
        return -1;

    return timeSchedule[index];
}

int MashSchedule::getCurrentTime()
{
    return timeSchedule[currentIndex];
}

QString MashSchedule::getName( int index )
{
    if( index >= count() )
        return "";

    return nameSchedule[index];
}

QString MashSchedule::getCurrentName()
{
    return nameSchedule[currentIndex];
}

void MashSchedule::clear()
{
    tempSchedule.clear();
    timeSchedule.clear();
    nameSchedule.clear();
}

void MashSchedule::addEntry(double temp, int time, QString name)
{
    tempSchedule.append( temp );
    timeSchedule.append( time );
    nameSchedule << name;
}

bool MashSchedule::setTemp( int index, double value )
{
    if( index >= (count() - 1) )
        return false;

    tempSchedule[index] = value;

    return true;
}

bool MashSchedule::setTime( int index, int value )
{
    if( index >= (count() - 1) )
        return false;

    timeSchedule[index] = value;

    return true;
}

bool MashSchedule::setName( int index, QString value )
{
    if( index >= (count() - 1) )
        return false;

    nameSchedule[index] = value;

    return true;
}
