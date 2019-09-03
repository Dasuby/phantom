#include "phantom.h"
#include <QFile.h>

Phantom::Phantom(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	QFile *qssFile;//�����ļ�
				   //this->setWindowFlags(Qt::FramelessWindowHint|Qt::Tool );
	qssFile = new QFile(":/Phantom/Resources/stylesheet.qss", this);
	qssFile->open(QFile::ReadOnly);// ֻ����ʽ�򿪸��ļ�
	QString styleSheet = tr(qssFile->readAll());// ��ȡ�ļ�ȫ�����ݣ�ʹ��tr()��������ת��ΪQString����
	qApp->setStyleSheet(styleSheet);// ΪQApplication������ʽ��
	qssFile->close();

	phantomport = new SerialPort(&ui);
	tcpclient = new TcpClient(&ui);
	wave = new Wave(&ui);
	baidumap = new BaiduMap(&ui);
	videotcpclient = new TcpClientVideo(&ui);
}
