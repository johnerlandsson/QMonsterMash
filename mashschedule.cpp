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

    if( role == Qt::DisplayRole )
    {
        qDebug() << "displayRole";
    }
    else if( role == Qt::EditRole )
    {
        double temp;
        int time;
        QString name;

        switch( index.column() )
        {
            case 0:
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

            case 1:
                time = value.toInt();
                if( time > 999 )
                    time = 999;
                else if( time < 1 )
                    time = 1;
                times[index.row()] = time;
                emit dataChanged( index, index );
                ret = true;
                break;

            case 2:
                name = value.toString();
                if( name.length() < 1 )
                    name = "Unnamed rest";
                else if( name.length() > 20 )
                    name = "Rest with long name";
                names[index.row()] = name;
                emit dataChanged( index, index );
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
        case 0:
            return "Temperature";
            break;
        case 1:
            return "Time";
            break;
        case 2:
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

QVariant MashSchedule::data( const QModelIndex &index, int role ) const
{
    if( !index.isValid() )
        return QVariant();

    QVariant ret;

    if( role == Qt::EditRole )
    {
        switch( index.column() )
        {
            case 0:     //Temp
                ret = temps[index.row()];
                break;
            case 1:     //Time
                ret = times[index.row()];
                break;
            case 2:     //Name
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
            case 0:
                ret = QString::number( temps[index.row()], 'f', 2 ) + QString::fromUtf8( "\u00B0" );
                break;
            case 1:
                ret = QString( "%1 min" ).arg( times[index.row()] );
                break;
            case 2:
                ret = names[index.row()];
                break;
            default:
                break;
        }
    }

    return ret;
}
