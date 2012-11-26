#ifndef PLOTSTATUSBAR_H
#define PLOTSTATUSBAR_H

#include <QStatusBar>

namespace Ui {
    class PlotStatusBar;
}

class PlotStatusBar : public QWidget
{
    Q_OBJECT

public:
    explicit PlotStatusBar( QWidget *parent = 0 );
    ~PlotStatusBar();

public slots:
        void setOutput( QString newValue );
        void setPv( QString newValue );
        void setSv( QString newValue );
        void setTotalTime( int time );
        void incrementProgress();

private:
    Ui::PlotStatusBar *ui;
    int timeProgress;
};

#endif // PLOTSTATUSBAR_H
