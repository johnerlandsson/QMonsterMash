#ifndef PLOTDIALOG_H
#define PLOTDIALOG_H

#include <QDialog>
#include <QTimer>

namespace Ui {
    class PlotDialog;
}

class PlotDialog : public QDialog
{
        Q_OBJECT
        
    public:
        explicit PlotDialog( QWidget *parent = 0 );
        ~PlotDialog();
        
    private:
        Ui::PlotDialog *ui;
        QTimer *sampleTimer;
        int sampleCounter;

    signals:
        void startManual( double value );
        void stopManual();
    private slots:
        void on_buttStart_clicked();
        void on_buttBrowse_clicked();
        void on_buttStop_clicked();
        void takeSample();
};

#endif // PLOTDIALOG_H
