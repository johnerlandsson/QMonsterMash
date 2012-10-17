QMonsterMash - PID Controller for your mashtun.

Author: John Erlandsson
License: GPLv2

Requirements:
    IgH's EtherCAT Master. - Get it at www.etherlab.org
    1 Beckhoff EK1100 - Bus coupler
    1 Beckhoff EL3202 - Analog input (PT100)
    1 Beckhoff EL2004 - Digital inputs

Electrical connection:
    PT100 temperatur sensor - Connect to EL3202 first input
    Heating element via solidstate relay - Connect to EL2004 output1
    Circulation pump via relay - Connect to EL2004 output2

Features:
    PID regulator for mash temperatures
    Easy to use mash schedule
    Plot temperatures
    Boil timer
    Hydrometer correction calculator

Description:
    This application is in an early stage of development. Please e-mail suggestions and questions to john@lunatech.se
