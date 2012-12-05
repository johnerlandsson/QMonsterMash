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

#include "completinglineeditdelegate.h"
#include <QLineEdit>

CompletingLineEditDelegate::CompletingLineEditDelegate( QObject *parent, WordfileStringListModel *model ) : QItemDelegate( parent ), wfslm( model )
{

}

QWidget *CompletingLineEditDelegate::createEditor( QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &/*index*/ ) const
{
    QLineEdit *editor = new QLineEdit( parent );
    QCompleter *completer = new QCompleter( editor );
    completer->setModel( wfslm );
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
