
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow)
{
    restoreWindowState();
    ui->setupUi(this);
    findLocalIPs();
    loadSettings();


    connect(ui->button_TcpServer, SIGNAL(clicked()), this, SLOT(onTcpServerButtonClicked()));
}

void MainWindow::onTcpServerButtonClicked()
{
    disconnect(ui->button_TcpServer, SIGNAL(clicked()), this, SLOT(onTcpServerButtonClicked()));

    if (setupConnection(TCPSERVER))
    {
        ui->statusBar->showMessage("Слушаем " + localAddr.toString() + ": " + QString::number(tcpServerListenPort), 0);
        connect(ui->button_TcpServer, SIGNAL(clicked()), this, SLOT(onTcpServerStopButtonClicked()));
        ui->button_TcpServer->setText("Стоп");
        ui->lineEdit_TcpServerListenPort->setDisabled(true);
        connect(mytcpserver, SIGNAL(myServerConnected(QString, quint16)), this, SLOT(onTcpServerNewConnection(QString, quint16)));
    }
    else
    {
        ui->statusBar->showMessage("Ошибка прослушивания " + localAddr.toString() + ": " + QString::number(tcpServerListenPort), 2000);
        connect(ui->button_TcpServer, SIGNAL(clicked()), this, SLOT(onTcpServerButtonClicked()));
    }

    saveSettings();
}

void MainWindow::onTcpServerNewConnection(const QString &from, quint16 port)
{
    ui->statusBar->showMessage("Соединен с " + from + ": " + QString::number(port), 0);
    disconnect(mytcpserver, SIGNAL(myServerConnected(QString, quint16)), this, SLOT(onTcpServerNewConnection(QString, quint16)));

    disconnect(ui->button_TcpServer, SIGNAL(clicked()), this, SLOT(onTcpServerStopButtonClicked()));
    ui->button_TcpServer->setText("Обрыв соединия");
    connect(ui->button_TcpServer, SIGNAL(clicked()), this, SLOT(onTcpServerDisconnectButtonClicked()));

    connect(mytcpserver, SIGNAL(myServerDisconnected()), this, SLOT(onTcpServerDisconnected()));
    connect(mytcpserver, SIGNAL(newMessage(QString, QString)), this, SLOT(onTcpServerAppendMessage(QString, QString)));

}

void MainWindow::onTcpServerStopButtonClicked()
{
    disconnect(ui->button_TcpServer, SIGNAL(clicked()), this, SLOT(onTcpServerStopButtonClicked()));

    ui->statusBar->showMessage("Стоп", 2000);
    disconnect(mytcpserver, SIGNAL(myServerConnected(QString, quint16)));
    mytcpserver->stopListening();
    ui->button_TcpServer->setText("Старты");
    ui->lineEdit_TcpServerListenPort->setDisabled(false);
    connect(ui->button_TcpServer, SIGNAL(clicked()), this, SLOT(onTcpServerButtonClicked()));
}

void MainWindow::onTcpServerDisconnectButtonClicked()
{
    mytcpserver->stopConnection();
}

void MainWindow::onTcpServerDisconnected()
{
    ui->statusBar->showMessage("Клиент отсоединился, слушать родолжаю " + localAddr.toString() + ": " + QString::number(tcpServerListenPort), 0);
    disconnect(ui->button_TcpServer, SIGNAL(clicked()), this, SLOT(onTcpServerDisconnectButtonClicked()));
    disconnect(mytcpserver, SIGNAL(myServerDisconnected()), this, SLOT(onTcpServerDisconnected()));
    disconnect(mytcpserver, SIGNAL(newMessage(QString, QString)), this, SLOT(onTcpServerAppendMessage(QString, QString)));

    connect(ui->button_TcpServer, SIGNAL(clicked()), this, SLOT(onTcpServerStopButtonClicked()));
    ui->button_TcpServer->setText("Стоп");
    connect(mytcpserver, SIGNAL(myServerConnected(QString, quint16)), this, SLOT(onTcpServerNewConnection(QString, quint16)));
}


void MainWindow::onTcpServerAppendMessage(const QString &from, const QString &message)
{
    std::string str = message.toStdString();
    int N = str.length();

//    а, у, о, ы, и, э, я, ю, ё, е
    for (int i = 0; i < N; i++) {
//        if (str[i] == 'a' || str[i] == 'у' || str[i] == 'o' || str[i] == 'ы' || str[i] == 'и' || str[i] == 'э' || str[i] == 'я' || str[i] == 'ю' || str[i] == 'ё' || str[i] == 'е')
          if (str[i] == 'a' || str[i] == 'e' || str[i] == 'i' || str[i] == 'o' || str[i] == 'u') {
            str[i] = str[i] - 'a' + 'A';
        }
    }

    QFile fileX("2.txt");
    QTextStream outstream2 (&fileX) ;
    if (fileX.open(QIODevice::WriteOnly | QFile::Text))
    {


        fileX.write(str.c_str());

    }
    fileX.close();


}


bool MainWindow::setupConnection(quint8 type)
{
    bool isSuccess = false;


    switch (type)
    {
    case TCPSERVER:
        tcpServerListenPort = ui->lineEdit_TcpServerListenPort->text().toInt();
        if (mytcpserver == nullptr)
        {
            mytcpserver = new MyTCPServer;
        }
        isSuccess = mytcpserver->listen(localAddr, tcpServerListenPort);
        break;
    }
    return isSuccess;
}

void MainWindow::findLocalIPs()
{
    interfaceList.clear();
    QList<QNetworkInterface> listInterface = QNetworkInterface::allInterfaces();
    for (int i = 0; i < listInterface.size(); ++i)
    {

        if (listInterface.at(i).humanReadableName().contains("Wi-Fi") || listInterface.at(i).humanReadableName().contains("wlp"))
        {
            interfaceList.append(listInterface.at(i));
        }
    }


}

void MainWindow::loadSettings()
{
    settingsFileDir = QApplication::applicationDirPath() + "/config.ini";
    QSettings settings(settingsFileDir, QSettings::IniFormat);

    ui->lineEdit_TcpServerListenPort->setText(settings.value("TCP_SERVER_LISTEN_PORT", 1234).toString());


}

void MainWindow::saveSettings()
{
    QSettings settings(settingsFileDir, QSettings::IniFormat);

    settings.setValue("TCP_SERVER_LISTEN_PORT", ui->lineEdit_TcpServerListenPort->text());

    settings.sync();
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings("ZPeng", "server");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    QMainWindow::closeEvent(event);
}

void MainWindow::restoreWindowState()
{
    QSettings settings("ZPeng", "server");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
}

MainWindow::~MainWindow()
{
    delete ui;
}
