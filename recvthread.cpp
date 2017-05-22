#include "recvthread.h"
#include "icmdhandler.h"
#include "cmdfactory.h"
#include "recvthread.h"
#include "devcmdrecv.h"
#include "phonecmdrecv.h"
#include "httphandler.h"
#include "platformcmdrecv.h"
#include "timerthread.h"
#include "const.h"

static char KEY_PLAT_BIND_PORT[] = "PlatformBindPort";

RecvThread::RecvThread()
    : m_bStartToEnd(false),
	  m_pCmdFactory(NULL),
	  m_pPlatformRecvSocket(NULL)
{

}

void RecvThread::addCmdRecv(PICmdRecv pCmdRecv)
{
    m_lstPICmdRecv.append(pCmdRecv);
}

void RecvThread::start( Priority /*= InheritPriority*/ )
{
	QSettings settings(Const::CONFIG_FILE_NAME, QSettings::IniFormat);
	bool isOk = false;
	quint16 platBindPort = settings.value(KEY_PLAT_BIND_PORT).toInt(&isOk);
	//Q_ASSERT(isOk);
	m_pPlatformRecvSocket = new QUdpSocket();
	m_pPlatformRecvSocket->bind(platBindPort);
	PlatformCmdRecv *pPlatCmdRecv = new PlatformCmdRecv(m_pCmdFactory, m_pPlatformRecvSocket);
	QObject::connect(m_pPlatformRecvSocket, &QUdpSocket::readyRead, pPlatCmdRecv, &PlatformCmdRecv::createCmd);
	addCmdRecv(pPlatCmdRecv);
	m_pPlatformRecvSocket->moveToThread(this);
	QThread::start();
}

void RecvThread::setCmdFactory(CmdFactory *pCmdFactory)
{
	m_pCmdFactory = pCmdFactory;
}

RecvThread::~RecvThread()
{
	delete m_pPlatformRecvSocket;
}

