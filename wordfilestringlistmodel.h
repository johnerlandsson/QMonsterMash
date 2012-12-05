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

#ifndef WORDFILESTRINGLISTMODEL_H
#define WORDFILESTRINGLISTMODEL_H

#include <QStringListModel>

class WordfileStringListModel : public QStringListModel
{
    Q_OBJECT
public:
    explicit WordfileStringListModel( QObject *parent = 0, const QString filePath = QString() );
    void loadFile( const QString fp );

signals:

public slots:

};

#endif // WORDFILESTRINGLISTMODEL_H
