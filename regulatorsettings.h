#ifndef REGULATORSETTINGS_H
#define REGULATORSETTINGS_H

#include <QWidget>

namespace Ui {
    class RegulatorSettings;
}

class RegulatorSettings : public QWidget
{
    Q_OBJECT

public:
    explicit RegulatorSettings(QWidget *parent = 0);
    ~RegulatorSettings();

private slots:
    void on_buttSave_clicked();

private:
    Ui::RegulatorSettings *ui;
};

#endif // REGULATORSETTINGS_H
