#   Copyright (C) 2012 John Erlandsson
#
#   This program is free software; you can redistribute it and/or
#   modify it under the terms of the GNU General Public License
#   as published by the Free Software Foundation; either version 2
#   of the License, or (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program; if not, write to the Free Software
#   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.


QT       += core gui phonon

TARGET   = QMonsterMash
TEMPLATE = app

DEFINES +=  "MAJOR_VERSION=0" \
            "MINOR_VERSION=3"

SOURCES +=  main.cpp\
            qmonstermash.cpp \
            pwmthread.cpp \
            iothread.cpp \
            hydrometercorrectionwidget.cpp \
            regulator.cpp \
            regulatorsettings.cpp \
            plotdialog.cpp \
            plotstatusbar.cpp \
            mashscheduledialog.cpp \
            doublespinboxdelegate.cpp \
            spinboxdelegate.cpp \
            boiltimerdialog.cpp \
            mashschedule.cpp \
            boilschedule.cpp \
            completinglineeditdelegate.cpp \
            wordfilestringlistmodel.cpp \
    dilutioncalcdialog.cpp

HEADERS  += qmonstermash.h \
            pwmthread.h \
            iothread.h \
            hydrometercorrectionwidget.h \
            regulator.h \
            regulatorsettings.h \
            plotdialog.h \
            plotstatusbar.h \
            mashschedule.h \
            doublespinboxdelegate.h \
            spinboxdelegate.h \
            boiltimerdialog.h \
            mashscheduledialog.h \
            boilschedule.h \
            completinglineeditdelegate.h \
            wordfilestringlistmodel.h \
    dilutioncalcdialog.h

FORMS    += qmonstermash.ui \
            hydrometercorrectionwidget.ui \
            regulatorsettings.ui \
            plotdialog.ui \
            plotstatusbar.ui \
            mashscheduledialog.ui \
            boiltimerdialog.ui \
    dilutioncalcdialog.ui

unix:!macx:!symbian: LIBS +=    -lethercat \
                                -lkdeui \
                                -L/usr/lib/kde4/devel/

INCLUDEPATH +=  /usr/include/kde4/

RESOURCES +=    resource.qrc

OTHER_FILES +=  README \
                LICENSE \
                sounds/air-raid-siren-alert.mp3 \
                INSTALL \
                scilab/qmonstermash.sce \
                images/Mash.svg \
                images/Pump.svg \
                images/startMash.png \
                images/startPump.png \
                images/stopMash.png \
                images/stopPump.png \
                boilwords.txt \
    mashwords.txt

target.path = /opt/QMonsterMash
INSTALLS += target

DEFINES +=  "PLOT_MAX_Y=110" \
            "PLOT_START_X=60"


#Edit these values to suite your system
DEFINES +=  "BusCouplerAlias=0" \
            "BusCouplerPos=0" \
            "AnaInAlias=0" \
            "AnaInPos=1" \
            "DigOutAlias=0" \
            "DigOutPos=2"













































