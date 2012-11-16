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
    QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
    spinBox->interpretText();
    int value = spinBox->value();

    model->setData( index, value, Qt::EditRole );
}

void DoubleSpinBoxDelegate::updateEditorGeometry( QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */ ) const
{
    editor->setGeometry( option.rect );
}
