#include "BaiduMap.h"

#include <QDir>
#include <QWebChannel>
#include "bridge.h"

BaiduMap::BaiduMap(Ui::PhantomClass *m_ui)
{
	ui = m_ui;

	webChannelInit();

	allConnectInit();
}


BaiduMap::~BaiduMap()
{
}

void BaiduMap::webChannelInit()
{
	QWebChannel *channel = new QWebChannel(this);

	m_bridge = (QObject*)bridge::instance();

	channel->registerObject("bridge", m_bridge);
	ui->baiduMapWebEngineView->page()->setWebChannel(channel);
	ui->baiduMapWebEngineView->page()->load(QUrl(QString("file:///%1/%2").arg(QApplication::applicationDirPath()).arg("map.html")));
	ui->baiduMapWebEngineView->show();
}

//�źŲ۰�
void BaiduMap::allConnectInit()
{
	//��bridge���յ�����ƶ��źţ�����C++�еĲۺ���
	connect(m_bridge, SIGNAL(mouseMoveEmit(QString, QString)), this, SLOT(getFromHtmlMouseMove(QString, QString)));
	//��bridge���յ����þ�γ�����źţ�����C++�еĲۺ���
	connect(m_bridge, SIGNAL(htmlToCppLocationEmit(QString, QString)), this, SLOT(getFromHtmlLocation(QString,QString)));

	//Ҳ����ֱ�ӽ���ť�ź���JS�еĺ�������
	//connect(ui->baiduMapStreetButton, &QPushButton::clicked, [this]() {
	//	ui->baiduMapWebEngineView->page()->runJavaScript("clearOverlaysPC()");
	//});
	//��ť���źŲ۰�
	connect(ui->baiduMapSetelliteButton, &QPushButton::clicked, this, &BaiduMap::on_baiduMapSetelliteButton_clicked);
	connect(ui->baiduMapStreetButton, &QPushButton::clicked, this, &BaiduMap::on_baiduMapStreetButton_clicked);
	connect(ui->baiduMapClearButton, &QPushButton::clicked, this, &BaiduMap::on_baiduMapClearButton_clicked);
}

//���õľ�γ
void BaiduMap::getFromHtmlLocation(QString lon, QString lat)
{
	ui->baiduMapLocateBrowser->append(lon + ", " + lat);
}
//��꾭γ
void BaiduMap::getFromHtmlMouseMove(QString lon, QString lat)
{
	ui->baiduMapLonLatLabel->setText(QString::fromLocal8Bit("��꾭�ȣ�") + lon + QString::fromLocal8Bit(" ���γ�ȣ�") + lat);
}

//����Ϊ�ֵ�ͼ
void BaiduMap::on_baiduMapStreetButton_clicked()
{
	ui->baiduMapWebEngineView->page()->runJavaScript("setStreetMapMode()");
}

//����Ϊ����ͼ
void BaiduMap::on_baiduMapSetelliteButton_clicked()
{
	ui->baiduMapWebEngineView->page()->runJavaScript("setSatelliteMapMode()");
}

//������б��
void BaiduMap::on_baiduMapClearButton_clicked()
{
	ui->baiduMapWebEngineView->page()->runJavaScript("clearOverlaysPC()");//����js������
	ui->baiduMapLocateBrowser->clear();//���text browser
}


