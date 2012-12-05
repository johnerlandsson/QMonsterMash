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

#include "doublespinboxdelegate.h"
#include <QDoubleSpinBox>

DoubleSpinBoxDelegate::DoubleSpinBoxDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

QWidget *DoubleSpinBoxDelegate::createEditor( QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex &/* index */ ) const
{
    QDoubleSpinBox *editor = new QDoubleSpinBox( parent );
    editor->setMinimum( 20.0f );
    editor->setMaximum( 80.0f );
    editor->setDecimals( 2 );
    editor->setSuffix( QString::fromUtf8( "\u00B0" ) );

    return editor;
}

void DoubleSpinBoxDelegate::setEditorData( QWidget *editor, const QModelIndex &index ) const
{
    double value = index.model()->data( index, Qt::EditRole ).toDouble();

    QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
    spinBox->setValue( value );
}

void DoubleSpinBoxDelegate::setModelData( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const
{
    QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
    spinBox->interpretText();
    double value = spinBox->value();

    model->setData( index, value, Qt::EditRole );
}

void DoubleSpinBoxDelegate::updateEditorGeometry( QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */ ) const
{
    editor->setGeometry( option.rect );
}
