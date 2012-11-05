#include "plotstatusbar.h"
#include "ui_plotstatusbar.h"

PlotStatusBar::PlotStatusBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlotStatusBar)
{
    ui->setupUi(this);

    ui->lblBlue->setStyleSheet( "QLabel{ background-color: blue }" );
    ui->lblGreen->setStyleSheet( "QLabel{ background-color: green }" );
    ui->lblYellow->setStyleSheet( "QLabel{ background-color: yellow }" );
}

PlotStatusBar::~PlotStatusBar()
{
    delete ui;
}
