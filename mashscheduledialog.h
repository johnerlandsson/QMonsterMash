#ifndef MASHSCHEDULEDIALOG_H
#define MASHSCHEDULEDIALOG_H

#include <QDialog>
#include "mashscheduletablemodel.h"

namespace Ui {
    class MashScheduleDialog;
}

class MashScheduleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MashScheduleDialog( QWidget *parent, MashSchedule ms = MashSchedule() );
    ~MashScheduleDialog();
    MashSchedule getSchedule();

signals:
    void newSchedule( MashSchedule newSched );

private slots:
    void resetSchedule();

    void on_buttNew_clicked();

private:
    Ui::MashScheduleDialog *ui;
    MashSchedule ret;
    MashScheduleTableModel *twMashScheduleModel;
};

#endif // MASHSCHEDULEDIALOG_H
