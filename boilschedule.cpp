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

#include "boilschedule.h"

BoilSchedule::BoilSchedule( QObject *parent ) :
    QAbstractTableModel(parent)
{
    appendRow( 60, 100, "Magnum" );
    appendRow( 15, 50, "Amarillo" );
    appendRow( 5, 50, "Amarillo" );
    appendRow( 15, 60, "Irish moss" );
    appendRow( 5, 2, "tsp", "Yeast nutrient" );

    sort();
}

QVariant BoilSchedule::data( const QModelIndex &index, int role ) const
{
    if( !index.isValid() )
        return QVariant();

    QVariant ret;

    if( role == Qt::DisplayRole )
    {
        switch( index.column() )
        {
            case Time:
                ret = QString( "%1 min" ).arg( times[index.row()] );
                break;

            case Amount:
                ret = QString( "%1 %2" ).arg( amounts[index.row()] ).arg( amountUnits[index.row()] );
                break;

            case Name:
                ret = names[index.row()];
                break;

            default:
                return QVariant();
                break;
        }
    }
    else if( role == Qt::EditRole )
    {
        switch( index.column() )
        {
            case Time:
                ret = times[index.row()];
                break;

            case Amount:
                ret = QString( "%1 %2" ).arg( amounts[index.row()] ).arg( amountUnits[index.row()] );
                break;

            case Name:
                ret = names[index.row()];
                break;

            default:
                return QVariant();
                break;
        }
    }

    return ret;
}

bool BoilSchedule::setData( const QModelIndex &index, const QVariant &value, int role )
{
    if( !index.isValid() )
        return false;

    if( role != Qt::EditRole )
        return false;

    int tmpi = value.toInt();
    QString tmps = value.toString();
    QStringList tmpsl = value.toString().split( ' ' );

    switch( index.column() )
    {
        case Time:
            if( tmpi < 0 || tmpi > 999 )
                return false;

            times[index.row()] = tmpi;
            sort();

            emit layoutChanged();
            break;

        case Amount:
            tmpi = tmpsl.at( 0 ).toInt();
            if( tmpi < 1 || tmpi > 999 )
                return false;
            amounts[index.row()] = tmpi;

            if( tmpsl.count() < 2 )
                    amountUnits[index.row()] = "g";
            else
            {
                tmps = tmpsl[1];
                amountUnits[index.row()] = tmps;
            }

            emit dataChanged( index, index );
            break;

        case Name:
            if( tmps.length() == 0 || tmps.length() == MAX_NAME_LENGTH )
                return false;

            names[index.row()] = tmps;

            emit dataChanged( index, index );
            break;

        default:
            return false;
            break;
    }
    return true;
}

Qt::ItemFlags BoilSchedule::flags( const QModelIndex &index ) const
{
    if( index.column() >= 0 && index.column() <= 2 )
        return Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable;

    return 0;
}

int BoilSchedule::getTime( int index )
{
    if( times.count() <= index )
        return -1;

    return times[index];
}

QString BoilSchedule::getName( int index )
{
    if( names.count() <= index )
        return QString();

    return names[index];
}

int BoilSchedule::columnCount( const QModelIndex &/*parent*/ ) const
{
    return 3;
}

int BoilSchedule::rowCount( const QModelIndex &/*parent*/ ) const
{
    return times.count();
}

void BoilSchedule::appendRow( int time, int amount, QString name )
{
    appendRow( time, amount, "g", name );
}

void BoilSchedule::appendRow( int time, int amount, QString amountUnit, QString name )
{
    times.append( time );
    amounts.append( amount );
    amountUnits.append( amountUnit );
    names.append( name );

    emit layoutChanged();
}

bool BoilSchedule::removeRow( int row, const QModelIndex &/*parent*/ )
{
    if( row < 0 || row >= this->rowCount() )
        return false;

    times.removeAt( row );
    names.removeAt( row );
    amounts.removeAt( row );
    amountUnits.removeAt( row );

    emit layoutChanged();

    return true;
}

QVariant BoilSchedule::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if( orientation != Qt::Horizontal || role != Qt::DisplayRole )
        return QVariant();

    switch( section )
    {
        case Time:
            return "Time";
            break;

        case Amount:
            return "Amount";
            break;

        case Name:
            return "Name";
            break;

        default:
            return QVariant();
            break;
    }
}

void BoilSchedule::sort()
{
    QList<int> tmpTime;
    QList<int> tmpAmount;
    QStringList tmpAmountUnit;
    QStringList tmpName;

    while( rowCount() > 0 )
    {
        int highest = 0;

        for( int i = 0; i < rowCount(); i++ )
        {
            if( times[i] > times[highest] )
                highest = i;
        }

        tmpTime.append( times[highest] );
        times.removeAt( highest );
        tmpAmount.append( amounts[highest] );
        amounts.removeAt( highest );
        tmpAmountUnit.append( amountUnits[highest] );
        amountUnits.removeAt( highest );
        tmpName.append( names[highest] );
        names.removeAt( highest );
    }

    times = tmpTime;
    amounts = tmpAmount;
    amountUnits = tmpAmountUnit;
    names = tmpName;
}
