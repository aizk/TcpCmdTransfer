#include "tcpthread.h"
#include "const.h"
#include "utils.h"
#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/helpers/stringhelper.h>
#include <log4cplus/helpers/sleep.h>
#include <log4cplus/loggingmacros.h>

using namespace log4cplus;

TcpThread::TcpThread()
{

}

void TcpThread::setTcpCmdFactory(TcpCmdFactory *pTcpCmdFactory)
{
    m_pTcpCmdFactory = pTcpCmdFactory;
}

TcpSocket *TcpThread::getTcpSocketByDevId(const quint32 devId)
{
    return m_mapDevIdToTcpSocket.value(devId);
}

void TcpThread::setTcpCmdRecv(TcpCmdRecv *pTcpCmdRecv)
{
    m_pTcpCmdRecv = pTcpCmdRecv;
}

void TcpThread::handleNewSocketDescriptor(qintptr socketDescriptor)
{
    TcpSocket *pNewTcpSock = new TcpSocket();
    pNewTcpSock->setSocketDescriptor(socketDescriptor);
	pNewTcpSock->setIsDev(m_isDev);
	LOG4CPLUS_WARN_FMT(
		Logger::getRoot(), 
		LOG4CPLUS_TEXT("new Connection from %s %s:%d"), 
		m_isDev ? Const::STR_DEV : Const::STR_APP,
		pNewTcpSock->peerAddress().toString().toUtf8().data(), 
		pNewTcpSock->peerPort()
		);
    QObject::connect(pNewTcpSock, &TcpSocket::dataReady, this, &TcpThread::handleCmdReady, Qt::DirectConnection);
	QObject::connect(pNewTcpSock, &TcpSocket::disconnect, this, &TcpThread::handleSocketDisconnect, Qt::DirectConnection);
	QObject::connect(pNewTcpSock, &TcpSocket::toDisconnect, this, &TcpThread::handleToDisconnect, Qt::QueuedConnection);
	QObject::connect(pNewTcpSock, &TcpSocket::disconnect, m_pTcpCmdFactory, &TcpCmdFactory::handleSocketDisconnect, Qt::DirectConnection);
    m_lstTcpSocket.append(pNewTcpSock);
	TcpSocket::insertTcpSocketToSet(pNewTcpSock);
}

void TcpThread::handleCmdReady(quint32 devId, TcpSocket * pTcpSocket, QByteArray data)
{
	if(!m_mapDevIdToTcpSocket.contains(devId))
	{
		m_mapDevIdToTcpSocket.insert(devId, pTcpSocket);
	}
	else
	{
		TcpSocket *pCurTcpSocket = m_mapDevIdToTcpSocket[devId];
		if(pCurTcpSocket && pCurTcpSocket != pTcpSocket)
		{
			pCurTcpSocket->disconnectFromHost();
			m_mapDevIdToTcpSocket[devId] = pTcpSocket;
		}
	}
    m_pTcpCmdRecv->handleCmd(devId, pTcpSocket, data);
}

TcpThread::~TcpThread()
{

}

void TcpThread::handleSocketDisconnect( quint32 devId, TcpSocket *pTcpSocket )
{
	
	if(devId)
	{
		if(pTcpSocket->isDev())
			Utils::remoteWinAlarm(devId);
		LOG4CPLUS_WARN_FMT(
			Logger::getRoot(), 
			LOG4CPLUS_TEXT("%s %u disconnect %s:%d"), 
			pTcpSocket->isDev() ? Const::STR_DEV : Const::STR_APP,
			devId,
			pTcpSocket->peerAddress().toString().toUtf8().data(), 
			pTcpSocket->peerPort()
			);
	}
	else
	{
		LOG4CPLUS_WARN_FMT(
			Logger::getRoot(), 
			LOG4CPLUS_TEXT("%s disconnect %s:%d"), 
			pTcpSocket->isDev() ? Const::STR_DEV : Const::STR_APP,
			pTcpSocket->peerAddress().toString().toUtf8().data(), 
			pTcpSocket->peerPort()
			);
	}
	TcpSocket *pCurTcpSocket = m_mapDevIdToTcpSocket[devId];
	if(pTcpSocket == pCurTcpSocket)
	{
		m_mapDevIdToTcpSocket.remove(devId);
	}
	TcpSocket::removeFromTcpSocketSet(pTcpSocket);
	pTcpSocket->deleteLater();
}

void TcpThread::setIsDev( bool isDev )
{
	m_isDev = isDev;
}

bool TcpThread::isDev()
{
	return m_isDev;
}

void TcpThread::handleToDisconnect( TcpSocket *pTcpSocket )
{
	pTcpSocket->disconnectFromHost();
}

