#ifndef DILUTIONCALCDIALOG_H
#define DILUTIONCALCDIALOG_H

#include <QDialog>

namespace Ui {
    class DilutionCalcDialog;
}

class DilutionCalcDialog : public QDialog
{
        Q_OBJECT
        
    public:
        explicit DilutionCalcDialog(QWidget *parent = 0);
        ~DilutionCalcDialog();
        
    private:
        Ui::DilutionCalcDialog *ui;

    private slots:
        void calculate();
};

#endif // DILUTIONCALCDIALOG_H
