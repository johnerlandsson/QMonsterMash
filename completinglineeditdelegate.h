#ifndef COMPLETINGLINEEDITDELEGATE_H
#define COMPLETINGLINEEDITDELEGATE_H

#include <QItemDelegate>
#include <QAbstractItemModel>
#include <QCompleter>

class CompletingLineEditDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit CompletingLineEditDelegate( QObject *parent = 0, QString wordfilePath = QString() );

    QWidget *createEditor( QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index ) const;
    void setEditorData( QWidget *editor, const QModelIndex &index ) const;
    void setModelData( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const;
    void updateEditorGeometry( QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index ) const;

private:
    QString wfp;


signals:

public slots:

};

#endif // COMPLETINGLINEEDITDELEGATE_H
