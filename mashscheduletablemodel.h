#ifndef MASHSCHEDULETABLEMODEL_H
#define MASHSCHEDULETABLEMODEL_H

#include <QStandardItemModel>
#include "mashschedule.h"

class MashScheduleTableModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit MashScheduleTableModel( QObject *parent = 0 );
    void setSchedule( MashSchedule newSched );
    MashSchedule getSchedule();
    void insertRow( int row, const QModelIndex &parent );

private:
    MashSchedule mashSchedule;
    QVariant data( const QModelIndex &index, int role ) const;
    bool setData( const QModelIndex &index, const QVariant &value, int role );

signals:


public slots:

};

#endif // MASHSCHEDULETABLEMODEL_H
