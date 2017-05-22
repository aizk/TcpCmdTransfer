#include "tcpserver.h"
#include "tcpcmdrecv.h"
#include "tcpsocket.h"
#include "tcpthread.h"
#include "const.h"
#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/helpers/stringhelper.h>
#include <log4cplus/helpers/sleep.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/thread/threads.h>

using namespace log4cplus;

TcpServer::TcpServer()
    : m_listenPort(0),
	  m_isDev(false)
{
}

void TcpServer::setTcpCmdRecv( TcpCmdRecv *pTcpCmdRecv )
{
    m_pTcpCmdRecv = pTcpCmdRecv;
}

void TcpServer::incomingConnection(qintptr socketDescriptor)
{
	static int iThread = 0;
	if(Const::MAX_TCP_THREAD == iThread)
	{
		iThread = 0;
	}
	TcpThread *pTcpThread = m_lstTcpThread[iThread++];
	QMetaObject::invokeMethod(
		pTcpThread,
		"handleNewSocketDescriptor",
		Qt::QueuedConnection,
		Q_ARG(qintptr, socketDescriptor)
		);
}

void TcpServer::createTcpThread()
{
	for(int i = 0; i < Const::MAX_TCP_THREAD; ++i)
	{
		TcpThread *pNewTcpThread = new TcpThread();
		pNewTcpThread->setIsDev(m_isDev);
		pNewTcpThread->setTcpCmdRecv(m_pTcpCmdRecv);
		pNewTcpThread->setTcpCmdFactory(m_pTcpCmdFactory);
		pNewTcpThread->start();
		pNewTcpThread->moveToThread(pNewTcpThread);
		m_lstTcpThread.append(pNewTcpThread);
	}
}

void TcpServer::setListenPort( quint16 listenPort )
{
	m_listenPort = listenPort;
}

bool TcpServer::startListen()
{
	bool rtn = this->listen(QHostAddress::Any, m_listenPort);
	if(rtn)
	{
		LOG4CPLUS_INFO_FMT(
			Logger::getRoot(), 
			LOG4CPLUS_TEXT("listening success on %d"),
			m_listenPort
			);
	}
	else
	{
		LOG4CPLUS_INFO_FMT(
			Logger::getRoot(), 
			LOG4CPLUS_TEXT("listen failed on %d"),
			m_listenPort
			);
	}
	return rtn;
}

TcpServer::~TcpServer()
{

}

void TcpServer::setTcpCmdFactory( TcpCmdFactory *pTcpCmdFactory )
{
	m_pTcpCmdFactory = pTcpCmdFactory;
}

void TcpServer::setIsDev( bool isDev )
{
	m_isDev = isDev;
}

bool TcpServer::isDev()
{
	return m_isDev;
}


