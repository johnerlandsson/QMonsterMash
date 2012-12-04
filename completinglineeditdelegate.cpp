#include "completinglineeditdelegate.h"
#include <QLineEdit>
#include "wordfilestringlistmodel.h"

CompletingLineEditDelegate::CompletingLineEditDelegate( QObject *parent, QString wordfilePath ) : QItemDelegate( parent ), wfp( wordfilePath )
{

}

QWidget *CompletingLineEditDelegate::createEditor( QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &/*index*/ ) const
{
    QLineEdit *editor = new QLineEdit( parent );
    QCompleter *completer = new QCompleter( editor );
    WordfileStringListModel *model = new WordfileStringListModel( completer, wfp );
    completer->setModel( model );
    completer->setModelSorting( QCompleter::CaseInsensitivelySortedModel );
    completer->setCaseSensitivity( Qt::CaseInsensitive );
    completer->setWrapAround( false );
    editor->setCompleter( completer );

    return editor;
}

void CompletingLineEditDelegate::setEditorData( QWidget *editor, const QModelIndex &index ) const
{
    QString value = index.data( Qt::EditRole ).toString();

    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);

    lineEdit->setText( value );
}

void CompletingLineEditDelegate::setModelData( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const
{
    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);

    model->setData( index, lineEdit->text(), Qt::EditRole );
}

void CompletingLineEditDelegate::updateEditorGeometry( QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/*index*/ ) const
{
    editor->setGeometry( option.rect );
}
