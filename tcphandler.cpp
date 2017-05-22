#include "tcphandler.h"
#include "const.h"

TcpHandler::TcpHandler(TcpSocket *pTcpSocket)
    : m_pTcpSocket(NULL),
	  m_isValid(false)
{
	if(pTcpSocket)
	{
		m_pTcpSocket = pTcpSocket;
		QObject::connect(this, &TcpHandler::toSendData, m_pTcpSocket, &TcpSocket::sendData);
	}
}

TcpHandler::~TcpHandler()
{

}

int TcpHandler::handleCmd(QByteArray &aCmd)
{
	if(!m_isValid || !m_pTcpSocket)
		return Const::RTN_ERROR;
	//QMetaObject::invokeMethod(m_pTcpSocket, "sendData", Qt::AutoConnection, Q_ARG(QByteArray, aCmd));
	emit toSendData(aCmd);
	return 0;
}

void TcpHandler::setValid( bool isValid )
{
	m_isValid = isValid;
	if(!isValid)
		m_pTcpSocket = NULL;
}

TcpSocket * TcpHandler::getTcpSocket()
{
	return m_pTcpSocket;
}

bool TcpHandler::isValid()
{
	return m_isValid;
}

void TcpHandler::setTcpSocket( TcpSocket *pTcpSocket )
{
	Q_ASSERT(pTcpSocket);
	if(pTcpSocket)
	{
		m_pTcpSocket = pTcpSocket;
		QObject::connect(this, &TcpHandler::toSendData, m_pTcpSocket, &TcpSocket::sendData);
	}
}

