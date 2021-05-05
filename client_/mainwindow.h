
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextTable>
#include <QScrollBar>
#include <QSettings>
#include <QNetworkInterface>


#include "mytcpclient.h"


#define TCPCLIENT 20


namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

  private slots:

    void onTcpClientButtonClicked();
    void onTcpClientNewConnection(const QString &from, quint16 port);
    void onTcpClientStopButtonClicked();
    void onTcpClientTimeOut();
    void onTcpClientDisconnectButtonClicked();
    void onTcpClientDisconnected();
    void onTcpClientSendMessage();


  public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

  protected:
    void closeEvent(QCloseEvent *event);

  private:
    Ui::MainWindow *ui;
    void loadSettings();
    void saveSettings();
    void findLocalIPs();
    bool setupConnection(quint8 type);

    void restoreWindowState();

    MyTCPClient *mytcpclient = nullptr;

    QHostAddress tcpClientTargetAddr;
    quint16 tcpClientTargetPort;

    QHostAddress localAddr;

    quint16 tcpServerListenPort;

    QString settingsFileDir;
    QList<QNetworkInterface> interfaceList;
    quint8 type;
    QByteArray iContents ;


};

#endif // MAINWINDOW_H
