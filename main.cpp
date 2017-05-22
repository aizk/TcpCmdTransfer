#include <QCoreApplication>
#include "icmdhandler.h"
#include "cmdfactory.h"
#include "recvthread.h"
#include "devcmdrecv.h"
#include "phonecmdrecv.h"
#include "httphandler.h"
#include "platformcmdrecv.h"
#include "timerthread.h"
#include "const.h"
#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/helpers/stringhelper.h>
#include <log4cplus/helpers/sleep.h>
#include <log4cplus/loggingmacros.h>
#include "tcpcmdfactory.h"
#include "tcpdevcmdrecv.h"
#include "tcpappcmdrecv.h"
#include <QtCore>

static char KEY_DEV_LISTEN_PORT[] = "DevListenPort";
static char KEY_APP_LISTEN_PORT[] = "AppListenPort";

int main(int argc, char *argv[])
{
	QScopedPointer<QSettings> pSettings(new QSettings(Const::CONFIG_FILE_NAME, QSettings::IniFormat));
	bool isOk = false;
	// 从配置文件中获取设备监听端口
	quint16 devListenPort = pSettings->value(KEY_DEV_LISTEN_PORT).toInt(&isOk);
	Q_ASSERT(isOk);
	// 从配置文件中获取APP监听端口
	quint16 appListenPort = pSettings->value(KEY_APP_LISTEN_PORT).toInt(&isOk);
	Q_ASSERT(isOk);
	// 配置HTTP Handler
	HTTPHandler::readSettings(*pSettings);

	// 日志系统初始化
	log4cplus::initialize();
	log4cplus::PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("log4cplus.properties"));

    QCoreApplication a(argc, argv);
	qRegisterMetaType<qintptr>("qintptr");

	const int threadCount = QThreadPool::globalInstance()->maxThreadCount();
	QThreadPool::globalInstance()->setMaxThreadCount(qMax(4, 2 * threadCount));

	TcpCmdFactory *pCmdFactory = new TcpCmdFactory();

	TcpDevCmdRecv *pTcpDevCmdRecv = new TcpDevCmdRecv(pCmdFactory);
	TcpAppCmdRecv *pTcpAppCmdRecv = new TcpAppCmdRecv(pCmdFactory);

	TcpServer *pDevTcpServer = new TcpServer();
	pDevTcpServer->setIsDev(true);
	pDevTcpServer->setTcpCmdRecv(pTcpDevCmdRecv);
	pDevTcpServer->setTcpCmdFactory(pCmdFactory);
	pDevTcpServer->createTcpThread();
	pDevTcpServer->setListenPort(devListenPort);
	pDevTcpServer->startListen();

	TcpServer *pAppTcpServer = new TcpServer();
	pAppTcpServer->setIsDev(false);
	pAppTcpServer->setTcpCmdRecv(pTcpAppCmdRecv);
	pAppTcpServer->setTcpCmdFactory(pCmdFactory);
	pAppTcpServer->createTcpThread();
	pAppTcpServer->setListenPort(appListenPort);
	pAppTcpServer->startListen();

	AlarmDev::setCmdFactory(pCmdFactory);

	QThread *pTcpServerThread = new QThread();
	pTcpServerThread->start();
	pDevTcpServer->moveToThread(pTcpServerThread);
	pAppTcpServer->moveToThread(pTcpServerThread);

	/*RecvThread *pPlatformRecvThread = new RecvThread();
	pPlatformRecvThread->setCmdFactory(pCmdFactory);
	pPlatformRecvThread->start();*/

	TimerThread *pTimerThread = new TimerThread();
	pTimerThread->start();
	pTimerThread->moveToThread(pTimerThread);
	
    return a.exec();
}

