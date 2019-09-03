#include "TcpClientVideo.h"



TcpClientVideo::TcpClientVideo(Ui::PhantomClass *m_ui)
{
	ui = m_ui;

	tcpClientInit();

	//ui->VideoTcpClientLabel->setGeometry(0, 0, 600, 600);

}


TcpClientVideo::~TcpClientVideo()
{
}

//����һ֡�Ƿ���ɣ��������ʾ
void TcpClientVideo::showVideo(QByteArray recvMesgByte)
{

	qint8 t = 0;

	if ((t = (recvMesgByte).indexOf("end")) != -1)
	{
		QString str = QString(recvMesgByte);
		QStringList list = str.split("e");
		QString len = list[0];
		qint32 length = len.toInt();

		//�жϽ��յĳ��Ⱥͷ��͵ĳ����Ƿ����
		if (length == pData.size())
		{
			image = pData;
			if (imageresult.loadFromData(image))
			{
				ui->VideoTcpClientLabel->setPixmap(imageresult);

				ui->VideoTcpClientLabel->update();
			}



			//qDebug()<< "receive pic:" << str << "#" << length << "#" << pData.size();
		}
		pData.clear();
	}
	else
	{
		pData.append((recvMesgByte));
	}

}

//TCP��ʼ������ťʧ�ܣ��źŲ۳�ʼ����
void TcpClientVideo::tcpClientInit()
{
	ui->tcpClientSendButton->setEnabled(false);

	tcpSocket = new QTcpSocket(this);

	tcpConnectInit();
}

//�źŲ۳�ʼ��
void TcpClientVideo::tcpConnectInit()
{
	QObject::connect(ui->videoTcpClientConnectButton, &QPushButton::clicked, this, &TcpClientVideo::on_videoTcpClientConnectButton_clicked);
	connect(tcpSocket, &QTcpSocket::readyRead, this, &TcpClientVideo::readMessageFromServer);
	connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectError(QAbstractSocket::SocketError)));

}

//���Ӱ�ť�¼�
void TcpClientVideo::on_videoTcpClientConnectButton_clicked()
{
	if ((ui->videoTcpClientConnectButton->text()) == QString::fromLocal8Bit("����"))
	{
		host = ui->videoTcpClientHostLineEdit->text();
		port = ui->videoTcpClientPortLineEdit->text();

		qDebug() << "host:" << host << "port:" << port;

		tcpSocket->connectToHost(host, port.toInt());

		ui->videoTcpClientConnectButton->setText(QString::fromLocal8Bit("�Ͽ�"));

		//qDebug() << "connect";
	}
	else
	{
		tcpSocket->abort();

		ui->videoTcpClientConnectButton->setText(QString::fromLocal8Bit("����"));
		//qDebug() << "disconnect";
	}
}

//��ȡ��Ϣ
void TcpClientVideo::readMessageFromServer()
{
	recvMesgByte = tcpSocket->readAll();
	//ui->videoTcpClientReceiveBrowser->setText(QString(recvMesgByte));
	showVideo(recvMesgByte);
}




//���Ӵ���
void TcpClientVideo::connectError(QAbstractSocket::SocketError)
{
	//qDebug() << "connect error:" << tcpSocket->errorString();

	tcpSocket->abort();

	ui->videoTcpClientConnectButton->setText(QString::fromLocal8Bit("����"));

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

