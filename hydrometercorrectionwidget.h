#ifndef HYDROMETERCORRECTIONWIDGET_H
#define HYDROMETERCORRECTIONWIDGET_H

#include <QWidget>

namespace Ui {
    class HydrometerCorrectionWidget;
}

class HydrometerCorrectionWidget : public QWidget
{
        Q_OBJECT
        
    public:
        explicit HydrometerCorrectionWidget(QWidget *parent = 0);
        ~HydrometerCorrectionWidget();
        
    private slots:
        void on_spnCalTemp_valueChanged(int arg1);

        void on_spnSgTemp_valueChanged(int arg1);

        void on_dspnSg_valueChanged(double arg1);

    private:
        Ui::HydrometerCorrectionWidget *ui;
        void recalculate();
};

#endif // HYDROMETERCORRECTIONWIDGET_H
