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

#include "iothread.h"
#include <QDebug>

IoThread::IoThread( QObject */*parent*/ )
{
    /* Process data */

    //EL3202
    domainRegs[0].alias = AnaInAlias;
    domainRegs[0].position = AnaInPos;
    domainRegs[0].vendor_id = 0x00000002;
    domainRegs[0].product_code = 0x0c823052;
    domainRegs[0].index = 0x6000;
    domainRegs[0].subindex = 1;
    domainRegs[0].offset = &offAnaIn;
    domainRegs[0].bit_position = NULL;

    //EL2004
    domainRegs[1].alias = DigOutAlias;
    domainRegs[1].position = DigOutPos;
    domainRegs[1].vendor_id = 0x00000002;
    domainRegs[1].product_code = 0x07d43052;
    domainRegs[1].index = 0x7000;
    domainRegs[1].subindex = 1;
    domainRegs[1].offset = &offDigOut;
    domainRegs[1].bit_position = NULL;

    domainRegs[2].alias = 0;
    domainRegs[2].position = 0;
    domainRegs[2].vendor_id = 0;
    domainRegs[2].product_code = 0;
    domainRegs[2].index = 0;
    domainRegs[2].subindex = 0;
    domainRegs[2].offset = NULL;
    domainRegs[2].bit_position = NULL;

    //qDebug() << "Configuring master...";

    ec_slave_config_t *sc;

    master = ecrt_request_master( 0 );
    if( !master )
    {
        fprintf( stderr, "Failed to create master." );
        exit( EXIT_FAILURE );
    }

    domain = ecrt_master_create_domain( master );
    if( !domain )
    {
        fprintf( stderr, "Failed to create domain." );
        exit( EXIT_FAILURE );
    }

    if( !( sc_ana_in = ecrt_master_slave_config( master, AnaInAlias, AnaInPos, Beckhoff_EL3202 ) ) )
    {
        fprintf( stderr, "Failed to get slave configuration" );
        exit( EXIT_FAILURE );
    }

    sc = ecrt_master_slave_config( master, BusCouplerAlias, BusCouplerPos, Beckhoff_EK1100 );
    if( !sc )
        exit( EXIT_FAILURE );

    if( ecrt_domain_reg_pdo_entry_list( domain, domainRegs ) )
    {
        fprintf( stderr, "PDO entry registration failed!\n" );
        exit( EXIT_FAILURE );
    }

    //Activating master
    //qDebug() << "Activating master";

    if( ecrt_master_activate( master ) )
    {
        fprintf( stderr, "Failed to activate master" );
        exit( EXIT_FAILURE );
    }

    if( !( domainPd = ecrt_domain_data( domain ) ) )
    {
        fprintf( stderr, "Failed to return process data" );
        exit( EXIT_FAILURE );
    }

    // Send and receive a couple of times to get things started...
    for( int i = 0; i < 3000; i++ )
    {
        ecrt_master_receive( master );
        ecrt_domain_process( domain );
        EC_WRITE_U8( domainPd + offDigOut, 0x00 );
        ecrt_domain_queue( domain );
        ecrt_master_send( master );
        usleep( 500 );
    }

    outp0Value = false;
    outp1Value = false;
    outp2Value = false;
    outp3Value = false;

    AnalogInput1Value = 0;
}


void IoThread::run()
{
    running = true;

    while( running )
    {
        ecrt_master_receive( master );
        ecrt_domain_process( domain );

        mutex.lock();

        AnalogInput1Value = (double)EC_READ_U16( domainPd + offAnaIn + 2 ) / 10;

        unsigned char tmpOutp = 0;
        if( outp0Value )
            tmpOutp = 1;
        if( outp1Value )
            tmpOutp += 2;
        if( outp2Value )
            tmpOutp += 4;
        if( outp3Value )
            tmpOutp += 8;

        mutex.unlock();

        EC_WRITE_U8( domainPd + offDigOut, tmpOutp );

        ecrt_domain_queue( domain );
        ecrt_master_send( master );

        usleep( 1000 );
    }
}

void IoThread::stop()
{
    running = false;
}

double IoThread::getAnalogInput1()
{
    mutex.lock();
    double ret = AnalogInput1Value;
    mutex.unlock();

    return ret;
}

void IoThread::setDigitalOutput0( bool value )
{
    mutex.lock();
    outp0Value = value;
    mutex.unlock();
}

bool IoThread::getDigitalOutput0()
{
    mutex.lock();
    bool ret = outp0Value;
    mutex.unlock();

    return ret;
}

void IoThread::setDigitalOutput1( bool value )
{
    mutex.lock();
    outp1Value = value;
    mutex.unlock();
}

bool IoThread::getDigitalOutput1()
{
    mutex.lock();
    bool ret = outp1Value;
    mutex.unlock();

    return ret;
}

void IoThread::setDigitalOutput2( bool value )
{
    mutex.lock();
    outp2Value = value;
    mutex.unlock();
}

bool IoThread::getDigitalOutput2()
{
    mutex.lock();
    bool ret = outp2Value;
    mutex.unlock();

    return ret;
}

void IoThread::setDigitalOutput3( bool value )
{
    mutex.lock();
    outp3Value = value;
    mutex.unlock();
}

bool IoThread::getDigitalOutput3()
{
    mutex.lock();
    bool ret = outp3Value;
    mutex.unlock();

    return ret;
}
