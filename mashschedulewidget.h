#ifndef MASHSCHEDULEWIDGET_H
#define MASHSCHEDULEWIDGET_H

#include <QWidget>

namespace Ui {
    class MashScheduleWidget;
}


class MashScheduleWidget : public QWidget
{
        Q_OBJECT

        
    public:
        struct mashEntry
        {
                double temp;
                int time;
                char name[17];

                struct mashEntry *next;
        };
        typedef struct mashEntry mashEntry_t;

    public:
        explicit MashScheduleWidget( QWidget *parent = 0 );
        ~MashScheduleWidget();
        mashEntry_t *getMashEntries();
        
    private slots:
        void on_chkAcidEnable_toggled(bool checked);
        void on_chkProteinEnable_toggled(bool checked);
        void on_chkSac2Enable_toggled(bool checked);
        void on_chkSac3Enable_toggled(bool checked);

    private:
        Ui::MashScheduleWidget *ui;
        mashEntry_t *mashEntries;
};

#endif // MASHSCHEDULEWIDGET_H
