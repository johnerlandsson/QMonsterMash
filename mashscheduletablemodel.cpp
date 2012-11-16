#include "mashscheduletablemodel.h"
#include <QDebug>

MashScheduleTableModel::MashScheduleTableModel( QObject *parent ) :
    QStandardItemModel( parent )
{
}

QVariant MashScheduleTableModel::data( const QModelIndex &index, int role ) const
{
    QVariant ret;

    if( role == Qt::DisplayRole )
    {
        QString dispValue;
        double dvalue = 0.0f;
        int ivalue = 0;

        switch( index.column() )
        {
            case 0: //Temp
                dvalue = QStandardItemModel::data( index, Qt::DisplayRole ).toDouble();
                dispValue = QString::number( dvalue, 'f', 2 ) + QString::fromUtf8( "\u00B0" );
                ret = dispValue;
                break;
            case 1: //Time
                ivalue = QStandardItemModel::data( index, Qt::EditRole ).toInt();
                dispValue = QString( "%1 min" ).arg( ivalue );
                ret = dispValue;
                break;
            default:
                ret = QStandardItemModel::data( index, Qt::DisplayRole );
                break;
        }
    }
    else
    {
        ret = QStandardItemModel::data( index, role );
    }

    return ret;
}

bool MashScheduleTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    bool ret;

    if( index.row() >= mashSchedule.count() )
        throw "MashScheduleTableModel setData: row out of bounds";

    if( role == Qt::EditRole )
    {
        QVariant data;

        switch( index.column() )
        {
            case 0: //Temp
                if( !mashSchedule.setTemp( index.row(), value.toDouble() ) )
                    ret = false;
                data = value;

                break;

            case 1: //Time
                if( !mashSchedule.setTime( index.row(), value.toInt() ) )
                    ret = false;
                data = value;

                break;

            case 2: //Name

                if( value.toString() == "" ) //Don't allow empty name value
                    data = "Rest";
                else
                    data = value;

                if( !mashSchedule.setName( index.row(), data.toString() ) )
                    ret = false;

                break;

            default:
                ret = false;
                break;
        }
        ret = QStandardItemModel::setData( index, data, role );
    }
    else
    {
        ret = QStandardItemModel::setData( index, value, role );
    }

    return ret;
}

void MashScheduleTableModel::insertRow( int row, const QModelIndex &parent )
{
    mashSchedule.addEntry( 0.0f, 0, "" );
    QStandardItemModel::insertRow( row, parent );
}

MashSchedule MashScheduleTableModel::getSchedule()
{
    return mashSchedule;
}

void MashScheduleTableModel::setSchedule( MashSchedule newSched )
{
    this->clear();

    for( int i = 0; i < newSched.count(); i++ )
    {
        QStandardItem *name = new QStandardItem;
        name->setData( newSched.getName( i ), Qt::EditRole );
        QStandardItem *temp = new QStandardItem;
        temp->setData( newSched.getTemp( i ), Qt::EditRole );
        QStandardItem *time = new QStandardItem;
        time->setData( newSched.getTime( i ), Qt::EditRole );

        this->setItem( i, 0, temp );
        this->setItem( i, 1, time );
        this->setItem( i, 2, name );
    }

    mashSchedule = newSched;
}
