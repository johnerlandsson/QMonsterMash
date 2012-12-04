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
