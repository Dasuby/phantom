#include "SerialPort.h"


SerialPort::SerialPort(Ui::PhantomClass *m_ui)
{
	ui = m_ui;

	serialInit();//���ڳ�ʼ��
	serialConnectInit();//�źŲ۳�ʼ��
}


SerialPort::~SerialPort()
{
}

//�źŲ۳�ʼ��
void SerialPort::serialConnectInit()
{
	QObject::connect(ui->serialOpenButton, &QPushButton::clicked, this, &SerialPort::on_serialOpenButton_clicked);
	QObject::connect(ui->serialClearReceiveButton, &QPushButton::clicked, this, &SerialPort::on_serialClearReceiveButton_clicked);
	QObject::connect(ui->serialSendButton, &QPushButton::clicked, this, &SerialPort::on_serialSendButton_clicked);
	QObject::connect(ui->serialClearSendButton, &QPushButton::clicked, this, &SerialPort::on_serialClearSendButton_clicked);
	connect(ui->serialTimeAdd, SIGNAL(stateChanged(int)), this, SLOT(onTimeStateChanged(int)));
	connect(ui->serialLineChange, SIGNAL(stateChanged(int)), this, SLOT(onLineStateChanged(int)));
}

void SerialPort::serialInit()
{
	serialCheck();
	//���ò����������˵�Ĭ����ʾ������
	ui->serialBondBox->setCurrentIndex(7);
	//�رշ��Ͱ�ť��ʹ��
	ui->serialSendButton->setEnabled(false);

	
}
void SerialPort::serialCheck()
{
	//qDebug() << "check com can use";
	ui->serialComSelectBox->clear();
	//���ҿ��õĴ���
	foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
	{
		QSerialPort serial;
		serial.setPort(info);
		if (serial.open(QIODevice::ReadWrite))
		{
			//ui->serialBox->addItem(serial.portName());
			ui->serialComSelectBox->addItem(serial.portName());
			serial.close();
		}
	}
}
void SerialPort::on_serialOpenButton_clicked()
{
	if( (ui->serialOpenButton->text()) == QString::fromLocal8Bit("�򿪴���"))
	{
		serial = new QSerialPort;
		//���ô�����
		serial->setPortName(ui->serialComSelectBox->currentText());
		//�򿪴���
		serial->open(QIODevice::ReadWrite);
		//���ò�����
		serial->setBaudRate(ui->serialBondBox->currentText().toInt());
		//��������λ��
		switch (ui->serialDateBitBox->currentIndex())
		{
		case 8: serial->setDataBits(QSerialPort::Data8); break;
		default: break;
		}
		//������żУ��
		switch (ui->serialOddEvenBox->currentIndex())
		{
		case 0: serial->setParity(QSerialPort::NoParity); break;
		default: break;
		}
		//����ֹͣλ
		switch (ui->serialStopBitBox->currentIndex())
		{
		case 1: serial->setStopBits(QSerialPort::OneStop); break;
		case 2: serial->setStopBits(QSerialPort::TwoStop); break;
		default: break;
		}
		//����������
		serial->setFlowControl(QSerialPort::NoFlowControl);
		//�ر����ò˵�ʹ��
		ui->serialComSelectBox->setEnabled(false);
		ui->serialBondBox->setEnabled(false);
		ui->serialDateBitBox->setEnabled(false);
		ui->serialOddEvenBox->setEnabled(false);
		ui->serialStopBitBox->setEnabled(false);
		ui->serialSendButton->setEnabled(true);
		ui->serialOpenButton->setText(QString::fromLocal8Bit("�رմ���"));
		//�����źŲ�
		QObject::connect(serial, &QSerialPort::readyRead, this, &SerialPort::read_Data);
	}
	else
	{
		//�رմ���
		//serial->clear();
		serial->close();
		serial->deleteLater();
		//�ָ�����ʹ��
		ui->serialComSelectBox->setEnabled(true);
		ui->serialBondBox->setEnabled(true);
		ui->serialDateBitBox->setEnabled(true);
		ui->serialOddEvenBox->setEnabled(true);
		ui->serialStopBitBox->setEnabled(true);
		ui->serialSendButton->setEnabled(true);
		ui->serialOpenButton->setText(QString::fromLocal8Bit("�򿪴���"));
	}
}

//��ȡ���յ�������
void SerialPort::read_Data()
{
	QByteArray recvQBA = serial->readAll();
	QTextCodec *tc = QTextCodec::codecForName("GBK");
	QString recvQStr = tc->toUnicode(recvQBA);
	if (!recvQStr.isEmpty())
	{
		QString str = ui->serialReceiveBrowser->toPlainText();
		str += recvQStr;
		if (if_show_time)
		{
			str += "[";
			str += get_time();
			str += "]";
		}
		if (if_auto_enter)
		{
			str += "\r\n";
		}
		ui->serialReceiveBrowser->clear();
		ui->serialReceiveBrowser->append(str);

		qDebug() << str.length();
	}
	recvQStr.clear();

}

//��ս��մ���
void SerialPort::on_serialClearReceiveButton_clicked()
{
	ui->serialReceiveBrowser->clear();
}

//��������
void SerialPort::on_serialSendButton_clicked()
{
	QString sendmesg = ui->serialSendEdit->toPlainText();

	sendmesg.replace(QString("\n"), QString("\r\n"));

	serial->write(sendmesg.toLatin1());

}
//������ʹ���
void SerialPort::on_serialClearSendButton_clicked()
{
	ui->serialSendEdit->clear();
}

//�Ƿ��Զ�������ʾ
void SerialPort::onLineStateChanged(int state)
{
	if (state == Qt::Checked) // "ѡ��"
	{
		if_auto_enter = true;
	}
	else if (state == Qt::PartiallyChecked) // "��ѡ"
	{
		if_auto_enter = false;
	}
	else // δѡ�� - Qt::Unchecked
	{
		if_auto_enter = false;
	}
}
//�Ƿ����ʱ���
void SerialPort::onTimeStateChanged(int state)
{
	if (state == Qt::Checked) // "ѡ��"
	{
		//qDebug() << tr("ѡ��");
		if_show_time = true;
	}
	else if (state == Qt::PartiallyChecked) // "��ѡ"
	{
		if_show_time = false;
	}
	else // δѡ�� - Qt::Unchecked
	{
		if_show_time = false;
	}
}