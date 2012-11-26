#ifndef MASHSHEDULEDIALOG_H
#define MASHSHEDULEDIALOG_H
#include <QDialog>
#include "mashschedule.h"

namespace Ui {
    class MashScheduleDialog;
}

class MashScheduleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MashScheduleDialog( QWidget *parent, MashSchedule *ms = NULL );
    ~MashScheduleDialog();

signals:

private slots:
    void on_buttNew_clicked();
    void on_buttDel_clicked();

private:
    Ui::MashScheduleDialog *ui;
    MashSchedule *twMashScheduleModel;
};

#endif // MASHSHEDULEDIALOG_H
