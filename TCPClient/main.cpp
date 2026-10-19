#include "tcpclient.h"

#include <QApplication>
// #include "openwidget.h"
//#include "online.h"
//#include "friendWidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // friendWidget w;
    // w.show();
    // openwidget w;
    // w.show();
    TcpClient::getTcpclient().show();
    return QApplication::exec();
}
