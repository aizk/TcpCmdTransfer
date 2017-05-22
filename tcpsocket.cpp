#include "tcpsocket.h"
#include "tcpserver.h"
#include "utils.h"
#include "const.h"
#include <log4cplus/loggingmacros.h>

using namespace log4cplus;

QSet<TcpSocket *> TcpSocket::setTcpSocket;
QReadWriteLock TcpSocket::setLock;

TcpSocket::TcpSocket()
	: m_isDevIdGot(false),
	  m_isDev(false),
	  m_noDataTimes(0),
	  m_devId(0)
{
	QObject::connect(this, &QTcpSocket::readyRead, this, &TcpSocket::handleReadyRead);
	QObject::connect(this, &QTcpSocket::disconnected, this, &TcpSocket::handleDisconnected);
}

quint32 TcpSocket::getDevId()
{
    return m_devId;
}

void TcpSocket::handleReadyRead()
{
	this->setDataTimingStart();
    QByteArray data = this->readAll();
	Utils::logByteArray(data, true);
    m_CmdBuffer.putData(data);
    QByteArray cmd;
    forever{
        cmd = m_CmdBuffer.nextCompleteCmd();
        if(cmd.isNull())
            break;
		if(!m_isDevIdGot)
		{
			m_devId = Utils::getDevIdFromCmd(cmd);
			LOG4CPLUS_INFO_FMT(
				Logger::getRoot(), 
				LOG4CPLUS_TEXT("%s %u on %s:%d"), 
				m_isDev ? Const::STR_DEV : Const::STR_APP,
				m_devId, 
				this->peerAddress().toString().toUtf8().data(), 
				this->peerPort()
				);
			m_isDevIdGot = true;
		}
        emit dataReady(m_devId, this, cmd);
    }
}

void TcpSocket::sendData( const QByteArray &dataToSend )
{
	//QByteArray dataToSend(Const::FRAME_START_FLAG, Const::CMD_FRAME_START_FLAG_SIZE);
	//dataToSend.append(data);
	int bytesWrite = write(dataToSend);
	if(-1 == bytesWrite)
	{
		LOG4CPLUS_DEBUG_FMT(
			Logger::getRoot(), 
			LOG4CPLUS_TEXT("Send to %s %u error"), 
			m_isDev ? Const::STR_DEV : Const::STR_APP,
			m_devId
			);
		return;
	}
	Utils::logByteArray(dataToSend, false);
	/*LOG4CPLUS_DEBUG_FMT(
		Logger::getRoot(), 
		LOG4CPLUS_TEXT("Send %d bytes to %s %u : %s"), 
		bytesWrite,
		m_isDev ? Const::STR_DEV : Const::STR_APP,
		m_devId,
		Utils::toReadableCmd(dataToSend.toHex()).data()
		);*/
}

void TcpSocket::handleDisconnected()
{
	emit disconnect(m_devId, this);
}

void TcpSocket::setIsDev( bool isDev )
{
	m_isDev = isDev;
}

bool TcpSocket::isDev()
{
	return m_isDev;
}

int TcpSocket::insertTcpSocketToSet( TcpSocket *pTcpSocket )
{
	QWriteLocker locker(&setLock);
	setTcpSocket.insert(pTcpSocket);
	return 0;
}

void TcpSocket::removeFromTcpSocketSet( TcpSocket *pTcpSocket )
{
	QWriteLocker locker(&setLock);
	setTcpSocket.remove(pTcpSocket);
}

void TcpSocket::incrementAllTimer()
{
	QReadLocker locker(&setLock);
	foreach(TcpSocket *pTcpSocket, setTcpSocket)
	{
		pTcpSocket->incrementNoDataTimes();
	}
}

void TcpSocket::incrementNoDataTimes()
{
	if(Const::TCP_NO_DATA_TIME == ++m_noDataTimes)
	{
		m_noDataTimes = 0;
		emit toDisconnect(this);
	}
}

void TcpSocket::setDataTimingStart()
{
	m_noDataTimes = 0;
}

