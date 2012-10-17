#ifndef IOTHREAD_H
#define IOTHREAD_H

#include <ethercat/ecrt.h>
#include <QThread>
#include <QMutex>

#define Beckhoff_EK1100 0x00000002, 0x044c2c52
#define Beckhoff_EL3202 0x00000002, 0x0c823052
#define Beckhoff_EL2004 0x00000002, 0x07d43052

class IoThread : public QThread
{
    Q_OBJECT

    public:
        explicit IoThread( QObject *parent = 0 );

        void run();
        void stop();

        double getAnalogInput1();
        bool getDigitalOutput0();
        bool getDigitalOutput1();
        bool getDigitalOutput2();
        bool getDigitalOutput3();

    public slots:
        void setDigitalOutput0( bool value );
        void setDigitalOutput1( bool value );
        void setDigitalOutput2( bool value );
        void setDigitalOutput3( bool value );

    private:
        bool running;

        ec_master_t *master;
        ec_domain_t *domain;

        unsigned int offAnaIn;
        unsigned int offDigOut;

        ec_pdo_entry_reg_t domainRegs[3];
        ec_slave_config_t *sc_ana_in;

        uint8_t *domainPd;

        QMutex mutex;
        double AnalogInput1Value;
        bool outp0Value, outp1Value, outp2Value, outp3Value;
};

#endif // IOTHREAD_H
