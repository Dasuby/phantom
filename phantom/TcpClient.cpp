#include "TcpClient.h"



TcpClient::TcpClient(Ui::PhantomClass *m_ui)
{
	ui = m_ui;

	tcpClientInit();
}


TcpClient::~TcpClient()
{
}

//TCP��ʼ������ťʧ�ܣ��źŲ۳�ʼ����
void TcpClient::tcpClientInit()
{
	ui->tcpClientSendButton->setEnabled(false);

	tcpSocket = new QTcpSocket(this);

	tcpConnectInit();
}

//�źŲ۳�ʼ��
void TcpClient::tcpConnectInit()
{
	QObject::connect(ui->tcpClientConnectButton, &QPushButton::clicked, this, &TcpClient::on_tcpClientConnectButton_clicked);
	connect(tcpSocket, &QTcpSocket::readyRead, this, &TcpClient::readMessageFromServer);
	connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(connectError(QAbstractSocket::SocketError)));

}

//���Ӱ�ť�¼�
void TcpClient::on_tcpClientConnectButton_clicked()
{
	if ((ui->tcpClientConnectButton->text()) == QString::fromLocal8Bit("����"))
	{
		host = ui->tcpClientHostLineEdit->text();
		port = ui->tcpClientPortLineEdit->text();

		qDebug() << "host:" << host << "port:" << port;
		
		tcpSocket->connectToHost(host, port.toInt());

		ui->tcpClientHostLineEdit->setEnabled(false);
		ui->tcpClientPortLineEdit->setEnabled(false);
		ui->tcpClientSendButton->setEnabled(true);
		ui->tcpClientConnectButton->setText(QString::fromLocal8Bit("�Ͽ�"));

		//qDebug() << "connect";
	}
	else
	{
		tcpSocket->abort();

		ui->tcpClientHostLineEdit->setEnabled(true);
		ui->tcpClientPortLineEdit->setEnabled(true);
		ui->tcpClientSendButton->setEnabled(false);
		ui->tcpClientConnectButton->setText(QString::fromLocal8Bit("����"));
		//qDebug() << "disconnect";
	}
}

//��ȡ��Ϣ
void TcpClient::readMessageFromServer()
{
	recvMesgByte = tcpSocket->readAll();
	ui->tcpClientReceiveBrowser->setText(QString(recvMesgByte));
}




//���Ӵ���
void TcpClient::connectError(QAbstractSocket::SocketError)
{
	//qDebug() << "connect error:" << tcpSocket->errorString();

	tcpSocket->abort();
	ui->tcpClientHostLineEdit->setEnabled(true);
	ui->tcpClientPortLineEdit->setEnabled(true);
	ui->tcpClientSendButton->setEnabled(false);
	ui->tcpClientConnectButton->setText(QString::fromLocal8Bit("����"));
	
	//QMessageBox::about(ui->phantomTabWidget, QString::fromLocal8Bit("����"), tcpSocket->errorString());

	QString error = tcpSocket->errorString();

	if ("Connection refused" == error)
	{
		QMessageBox::about(ui->phantomTabWidget, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("Զ�̷�����δ����"));
	}
	else if ("The remote host closed the connection" == error)
	{
		QMessageBox::about(ui->phantomTabWidget, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("Զ�̷����������ж�"));
	}
	else
	{
		QMessageBox::about(ui->phantomTabWidget, QString::fromLocal8Bit("��ʾ"), error);
	}
}

