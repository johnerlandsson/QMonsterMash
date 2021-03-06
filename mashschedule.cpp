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

#include "mashschedule.h"
#include <QDebug>

MashSchedule::MashSchedule( QObject *parent ) : QAbstractTableModel( parent )
{
    iterator = 0;
    setupDefaultSchedule();
}

void MashSchedule::setupDefaultSchedule()
{
    temps.clear();
    names.clear();
    times.clear();

    temps << 35.0f << 50.0f << 68.0f << 78.0f;
    times << 15 << 15 << 60 << 10;
    names << "Acid rest" << "Protein rest" << "Saccrification rest" << "Sparge";
}

bool MashSchedule::next()
{
    bool ret = false;

    if( iterator < temps.count() )
    {
        iterator++;
        ret = true;
    }

    return ret;
}

int MashSchedule::getCurrentTime()
{
    return times[iterator];
}

double MashSchedule::getCurrentTemp()
{
    return temps[iterator];
}

QString MashSchedule::getCurrentName()
{
    return names[iterator];
}

int MashSchedule::getTotalTime()
{
    int ret = 0;

    for( int i = 0; i < times.count(); i++ )
        ret += times[i];

    return ret;
}

void MashSchedule::reset()
{
    iterator = 0;
    sort();
}

void MashSchedule::sort()
{
    QList<double> tmpTemp;
    QList<int> tmpTime;
    QStringList tmpName;

    while( rowCount() > 0 )
    {
        int highest = 0;

        for( int i = 0; i < rowCount(); i++ )
        {
            if( temps[i] > temps[highest] )
                highest = i;
        }

        tmpTemp.prepend( temps[highest] );
        temps.removeAt( highest );
        tmpTime.prepend( times[highest] );
        times.removeAt( highest );
        tmpName.prepend( names[highest] );
        names.removeAt( highest );
    }

    temps= tmpTemp;
    times = tmpTime;
    names = tmpName;
}

int MashSchedule::columnCount( const QModelIndex &/*parent*/ ) const
{
    return 3;
}

int MashSchedule::rowCount( const QModelIndex &/*parent*/ ) const
{
    return temps.count();
}

bool MashSchedule::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if( !index.isValid() )
        return false;

    bool ret = false;

    if( role == Qt::EditRole )
    {
        double temp;
        int time;
        QString name;

        switch( index.column() )
        {
            case Temperature:
                temp = value.toDouble();

                if( temp < 0 )
                    temp = 0;
                else if( temp > 99 )
                    temp = 99;
                temps[index.row()] = temp;
                emit dataChanged( index, index );

                sort();
                emit layoutChanged();

                ret = true;
                break;

            case Time:
                time = value.toInt();
                if( time > 999 )
                    time = 999;
                else if( time < 1 )
                    time = 1;
                times[index.row()] = time;
                emit dataChanged( index, index );
                ret = true;
                break;

            case Name:
                name = value.toString();
                if( name.length() < 1 )
                    name = "Unnamed rest";
                else if( name.length() > 20 )
                    name = "Rest with long name";
                names[index.row()] = name;
                emit dataChanged( index, index );
                ret = true;
                break;

            default:
                break;
        }
    }

    return ret;
}

Qt::ItemFlags MashSchedule::flags( const QModelIndex &index ) const
{
    if( !index.isValid() )
        return 0;

    return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant MashSchedule::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if( orientation != Qt::Horizontal )
        return QVariant();

    if( role != Qt::DisplayRole )
        return QVariant();

    switch( section )
    {
        case Temperature:
            return "Temperature";
            break;
        case Time:
            return "Time";
            break;
        case Name:
            return "Name";
            break;
        default:
            return QVariant();
            break;
    }
}

void MashSchedule::appendRow()
{
    temps << 60.0f;
    times << 30;
    names << "Rest";

    sort();

    emit layoutChanged();
}

bool MashSchedule::removeRow( int row, const QModelIndex &/*parent*/ )
{
    if( row < 0 || row >= rowCount() )
        return false;

    times.removeAt( row );
    temps.removeAt( row );
    names.removeAt( row );

    emit layoutChanged();

    return true;
}

QVariant MashSchedule::data( const QModelIndex &index, int role ) const
{
    if( !index.isValid() )
        return QVariant();

    QVariant ret;

    if( role == Qt::EditRole )
    {
        switch( index.column() )
        {
            case Temperature:     //Temp
                ret = temps[index.row()];
                break;
            case Time:     //Time
                ret = times[index.row()];
                break;
            case Name:     //Name
                ret = names[index.row()];
                break;
            default:
                break;
        }
    }
    else if( role == Qt::DisplayRole )
    {
        switch( index.column() )
        {
            case Temperature:
                ret = QString::number( temps[index.row()], 'f', 2 ) + QString::fromUtf8( "\u00B0" );
                break;
            case Time:
                ret = QString( "%1 min" ).arg( times[index.row()] );
                break;
            case Name:
                ret = names[index.row()];
                break;
            default:
                break;
        }
    }

    return ret;
}
