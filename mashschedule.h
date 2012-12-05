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

    enum columns
    {
        Temperature = 0,
        Time = 1,
        Name = 2
    };

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
