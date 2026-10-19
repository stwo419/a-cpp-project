#include "tcpclinet.h"
#include "ui_tcpclinet.h"

TcpClinet::TcpClinet(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TcpClinet)
{
    ui->setupUi(this);
}

TcpClinet::~TcpClinet()
{
    delete ui;
}
