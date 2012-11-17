#ifndef BOILSCHEDULETABLEMODEL_H
#define BOILSCHEDULETABLEMODEL_H

#include <QStandardItemModel>
#include "boilschedule.h"

class BoilScheduleTableModel : public QStandardItemModel
{
        Q_OBJECT
    public:
        explicit BoilScheduleTableModel( QObject *parent = 0 );
        void setSchedule( BoilSchedule newSchedule );
        BoilSchedule getSchedule();

    private:
        BoilSchedule boilSchedule;
        QVariant data( const QModelIndex &index, int role ) const;
        bool setData( const QModelIndex &index, const QVariant &value, int role );
        
    signals:
        
    public slots:
        
};

#endif // BOILSCHEDULETABLEMODEL_H
