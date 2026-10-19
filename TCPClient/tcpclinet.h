#ifndef TCPCLINET_H
#define TCPCLINET_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class TcpClinet;
}
QT_END_NAMESPACE

class TcpClinet : public QMainWindow
{
    Q_OBJECT

public:
    explicit TcpClinet(QWidget *parent = nullptr);
    ~TcpClinet() override;

private:
    Ui::TcpClinet *ui;
};
#endif // TCPCLINET_H
