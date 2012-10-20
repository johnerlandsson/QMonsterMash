#ifndef REGULATORSETTINGS_H
#define REGULATORSETTINGS_H

#include <QWidget>
#include <QSettings>

namespace Ui {
    class RegulatorSettings;
}

class RegulatorSettings : public QWidget
{
        Q_OBJECT

public:
        typedef struct
        {
                double I;
                double P;
                double Imax;
                double Imin;
                int cycleTime;
        }reg_para_t;

        reg_para_t getParameters();

        explicit RegulatorSettings( QWidget *parent = 0 );
        ~RegulatorSettings();

private slots:
        void on_buttSave_clicked();

private:
        Ui::RegulatorSettings *ui;
        QSettings settings;
};

#endif // REGULATORSETTINGS_H
