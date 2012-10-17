#include <QtGui/QApplication>
#include "qmonstermash.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMonsterMash w;

    QApplication::setApplicationName( "QMonsterMash" );

    w.show();
    
    return a.exec();
}
