#include "boilscheduletablemodel.h"
#include <QDebug>

BoilScheduleTableModel::BoilScheduleTableModel(QObject *parent) :
    QStandardItemModel(parent)
{
    setSchedule( boilSchedule );

    QStringList header;
    header << "Time" << "Name";

    this->setHorizontalHeaderLabels( header );
}

void BoilScheduleTableModel::setSchedule( BoilSchedule newSchedule )
{
    this->clear();

    for( int i = 0; i < newSchedule.count(); i++ )
    {
        QStandardItem *time = new QStandardItem;
        time->setData( newSchedule.getTime( i ), Qt::EditRole );
        QStandardItem *name = new QStandardItem;
        name->setData( newSchedule.getName( i ), Qt::EditRole );

        this->setItem( i, 0, time );
        this->setItem( i, 1, name );
    }

    boilSchedule = newSchedule;
}

BoilSchedule BoilScheduleTableModel::getSchedule()
{
    return boilSchedule;
}

QVariant BoilScheduleTableModel::data( const QModelIndex &index, int role ) const
{
    QVariant ret = QStandardItemModel::data( index, role );

    if( role == Qt::DisplayRole )
    {
        switch( index.column() )
        {
            case 0:
                ret = ret.toString() + "min";
                break;

            default:
                break;
        }
    }

    return ret;
}

bool BoilScheduleTableModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
    bool ret = true;

    if( index.row() >= boilSchedule.count() )
        throw "BoilScheduleTableModel setData: row out of bounds";

    if( role == Qt::EditRole )
    {
        QVariant data;

        switch( index.column() )
        {
            case 0:
                if( !boilSchedule.setTime( index.row(), value.toInt() ) )
                    ret = false;
                data = value;
                break;

            case 1:
                if( value.toString() == "" )
                    data = "Unnamed addition";
                else
                    data = value;

                if( !boilSchedule.setName( index.row(), data.toString() ) )
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
