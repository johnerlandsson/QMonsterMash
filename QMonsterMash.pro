#-------------------------------------------------
#
# Project created by John Erlandsson 2012-10-17T02:02:33
#
#-------------------------------------------------

QT       += core gui phonon

TARGET = QMonsterMash
TEMPLATE = app


SOURCES +=  main.cpp\
            qmonstermash.cpp \
            pwmthread.cpp \
            mashschedulewidget.cpp \
            iothread.cpp \
            hydrometercorrectionwidget.cpp \
            boiltimerwidget.cpp

HEADERS  += qmonstermash.h \
            pwmthread.h \
            mashschedulewidget.h \
            iothread.h \
            hydrometercorrectionwidget.h \
            boiltimerwidget.h

FORMS    += qmonstermash.ui \
            mashschedulewidget.ui \
            hydrometercorrectionwidget.ui \
            boiltimerwidget.ui

unix:!macx:!symbian: LIBS +=    -lethercat \
                                -lkdeui \
                                -L/usr/lib/kde4/devel/

INCLUDEPATH +=  /usr/include/kde4/

RESOURCES +=    resource.qrc

OTHER_FILES += \
    README \
    LICENSE \
    sounds/air-raid-siren-alert.mp3

target.path = /opt/QMonsterMash
INSTALLS += target

#Edit these values to suite your system
DEFINES +=  "BusCouplerAlias=0" \
            "BusCouplerPos=0" \
            "AnaInAlias=0" \
            "AnaInPos=1" \
            "DigOutAlias=0" \
            "DigOutPos=2"
