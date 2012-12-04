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
