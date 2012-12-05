#ifndef MASHSCHEDULE_H
#define MASHSCHEDULE_H

#include <QAbstractTableModel>
#include <QStringList>

class MashSchedule : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit MashSchedule( QObject *parent = 0 );
    QVariant data( const QModelIndex &index, int role ) const;
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const;
    bool setData( const QModelIndex &index, const QVariant &value, int role );
    int rowCount( const QModelIndex &parent = QModelIndex() ) const;
    void appendRow();
    bool removeRow( int row, const QModelIndex &parent = QModelIndex() );
    int columnCount( const QModelIndex &parent = QModelIndex() ) const;
    Qt::ItemFlags flags( const QModelIndex &index ) const;
    int getCurrentTime();
    double getCurrentTemp();
    QString getCurrentName();
    bool next();
    int getTotalTime();
    void reset();

signals:

public slots:

private:
    QList<int> times;
    QList<double> temps;
    QStringList names;
    int iterator;
    void sort();
    void setupDefaultSchedule();

};

#endif // MASHSCHEDULE_H
