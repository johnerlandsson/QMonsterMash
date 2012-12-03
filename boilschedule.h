#ifndef BOILSCHEDULE_H
#define BOILSCHEDULE_H

#include <QAbstractTableModel>
#include <QStringList>

#define MAX_NAME_LENGTH 20

class BoilSchedule : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit BoilSchedule( QObject *parent = 0 );
    QVariant data( const QModelIndex &index, int role ) const;
    bool setData( const QModelIndex &index, const QVariant &value, int role );
    int rowCount( const QModelIndex &parent = QModelIndex() ) const;
    int columnCount( const QModelIndex &parent = QModelIndex() ) const;
    Qt::ItemFlags flags( const QModelIndex &index ) const;
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const;

    int getTime( int index );
    QString getName( int index );

    void appendRow( int time, int amount, QString name );
    void appendRow( int time, int amount, QString amountUnit, QString name );


    enum columns
    {
        Time = 0,
        Amount = 1,
        Name = 2
    };

signals:

public slots:

private:
    void sort();
    QList<int>times;
    QList<int>dispTimes;
    QList<int>amounts;
    QList<QString>amountUnits;
    QStringList names;

};

#endif // BOILSCHEDULE_H
