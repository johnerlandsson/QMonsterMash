#ifndef BOILTIMERWIDGET_H
#define BOILTIMERWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include <QTimer>
#include <QTime>

namespace Ui {
    class BoilTimerWidget;
}

class BoilTimerWidget : public QWidget
{
        Q_OBJECT
        
    public:
        explicit BoilTimerWidget(QWidget *parent = 0);
        ~BoilTimerWidget();
        
    private slots:
        void on_buttStart_clicked();
        void on_buttAdd_clicked();
        void updateLblTime();

        void on_buttRemove_clicked();

    private:
        Ui::BoilTimerWidget *ui;
        bool running;
        QStandardItemModel *tblAdditionsModel;
        QTimer tmrUpdateLblTime;
        QTime timerStarted;
        void sortValidateModel();
        int currentAddition;
};

#endif // BOILTIMERWIDGET_H
