#pragma once

#include "ui_phantom.h"
#include <QtSerialPort>
#include <QtWidgets/QMainWindow>



class SerialPort : public QObject
{
	Q_OBJECT
public:
	SerialPort(Ui::PhantomClass *m_ui);
	~SerialPort();

private:
	Ui::PhantomClass *ui;
	QSerialPort *serial;
	bool if_show_time = false;//�Ƿ���ʾʱ��
	bool if_auto_enter = false;//�Ƿ��Զ�����
	bool if_serial_open = false;//�Ƿ�ʼ��������

private:
	//void serialCheck();
	void serialInit();
	void serialConnectInit();
	QString get_time()
	{
		QDateTime time = QDateTime::currentDateTime();
		QString timetoString = time.toString("yyyy-MM-dd hh:mm:ss");//yyyy-MM-dd hh:mm:ss dddd
		return timetoString;
	}
	
	//QTimer *timer = new QTimer(this);
	//void timeSerialCheck()//��ʱ����ʱ�����ô���
	//{
	//	//ֹͣ���ж�ʱ��
	//	if (timer->isActive())
	//	{
	//		timer->stop();
	//	}
	//	else
	//	{
	//		connect(timer, SIGNAL(timeout()), this, SLOT(serialCheck()));
	//		timer->start(1000);
	//	}
	//}



private slots:
	void serialCheck();
	void read_Data();
	void on_serialOpenButton_clicked();
	void on_serialClearReceiveButton_clicked();
	void on_serialSendButton_clicked();
	void on_serialClearSendButton_clicked();
	void onLineStateChanged(int state);
	void onTimeStateChanged(int state);


};

