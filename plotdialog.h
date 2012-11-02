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

#ifndef PLOTDIALOG_H
#define PLOTDIALOG_H

#include <QDialog>
#include <QTimer>
#include "iothread.h"
#include <QTextStream>
#include <QFile>

namespace Ui {
    class PlotDialog;
}

class PlotDialog : public QDialog
{
        Q_OBJECT
        
    public:
        explicit PlotDialog( QWidget *parent = 0, IoThread *io = NULL );
        ~PlotDialog();
        
    private:
        Ui::PlotDialog *ui;
        QTimer *sampleTimer;
        int sampleCounter;
        IoThread *ec;
        QString filePath;
        QTextStream plotFileStream;
        QFile file;

    signals:
        void startManual( double value );
        void stopManual();
        void startingPlot( double output );
        void stoppingPlot();
    private slots:
        void on_buttStart_clicked();
        void on_buttBrowse_clicked();
        void on_buttStop_clicked();
        void takeSample();
};

#endif // PLOTDIALOG_H
