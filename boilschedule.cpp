#include "boilschedule.h"

BoilSchedule::BoilSchedule()
{
    timeSchedule.append( 60 );
    nameSchedule.append( "Bittering hops" );
    timeSchedule.append( 15 );
    nameSchedule.append( "Flavour hops" );
    timeSchedule.append( 5 );
    nameSchedule.append( "Aroma hops" );
}

int BoilSchedule::count()
{
    if( nameSchedule.count() != timeSchedule.count() )
        return -1;

    return nameSchedule.count();
}

void BoilSchedule::appendRow( int time, QString name )
{
    nameSchedule.append( name );
    timeSchedule.append( time );
}

bool BoilSchedule::setName( int index, QString value )
{
    if( index >= count() )
        return false;

    nameSchedule[index] = value;

    return true;
}

bool BoilSchedule::setTime( int index, int value )
{
    if( index >= count() )
        return false;

    timeSchedule[index] = value;

    return true;
}

int BoilSchedule::getTime( int index )
{
    if( index >= count() )
        throw "BoilSchedule getTime: row out of bounds";

    return timeSchedule[index];
}

QString BoilSchedule::getName( int index )
{
    if( index >= count() )
        throw "BoilSchedule getName: row out of bounds";

    return nameSchedule[index];
}
