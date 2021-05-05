
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextTable>
#include <QScrollBar>
#include <QSettings>
#include <QNetworkInterface>
#include <QFile>
#include <QDebug>


#include "mytcpserver.h"

#define TCPSERVER 10


namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

  private slots:
    void onTcpServerButtonClicked();
    void onTcpServerNewConnection(const QString &from, quint16 port);
    void onTcpServerStopButtonClicked();
    void onTcpServerDisconnectButtonClicked();
    void onTcpServerDisconnected();
    void onTcpServerAppendMessage(const QString &from, const QString &message);



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

    QTextTableFormat tableFormat;

    MyTCPServer *mytcpserver = nullptr;

    QHostAddress tcpClientTargetAddr;
    quint16 tcpClientTargetPort;

    QHostAddress localAddr;

    quint16 tcpServerListenPort;

    quint16 udpListenPort;
    QHostAddress udpTargetAddr;
    quint16 udpTargetPort;

    QString settingsFileDir;
    QList<QNetworkInterface> interfaceList;
    quint8 type;
    QByteArray iContents ;

};

#endif // MAINWINDOW_H
