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

#include "wordfilestringlistmodel.h"
#include <QFile>
#include <QApplication>

WordfileStringListModel::WordfileStringListModel( QObject *parent, const QString filePath ) : QStringListModel( parent )
{
    loadFile( filePath );
}

void WordfileStringListModel::loadFile( const QString fp )
{
    QStringList words;

    QFile file( fp );
    if ( !file.open( QFile::ReadOnly ) )
    {
        this->setStringList( words );
        return;
    }

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    while( !file.atEnd() )
    {
        QByteArray line = file.readLine();
        if( !line.isEmpty() )
            words << line.trimmed();
    }

    QApplication::restoreOverrideCursor();

    this->setStringList( words );
}
