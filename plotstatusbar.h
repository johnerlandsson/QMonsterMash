#ifndef PLOTSTATUSBAR_H
#define PLOTSTATUSBAR_H

#include <QWidget>

namespace Ui {
    class PlotStatusBar;
}

class PlotStatusBar : public QWidget
{
    Q_OBJECT

public:
    explicit PlotStatusBar(QWidget *parent = 0);
    ~PlotStatusBar();

private:
    Ui::PlotStatusBar *ui;
};

#endif // PLOTSTATUSBAR_H
