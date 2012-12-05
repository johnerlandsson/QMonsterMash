/*  Copyright (C) 2012 John Erlandsson

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

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
