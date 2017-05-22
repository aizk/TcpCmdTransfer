#include "tcpappcmdrecv.h"
#include "tcpsocket.h"

TcpAppCmdRecv::TcpAppCmdRecv( CmdFactory *pCmdFactory ) : TcpCmdRecv(pCmdFactory)
{

}

void TcpAppCmdRecv::handleCmd( quint32 devId, TcpSocket * tcpSocket, QByteArray data )
{
	m_pCmdFactory->createFromAppCmd(
		devId, 
		data, 
		tcpSocket->localAddress(), 
		tcpSocket->localPort()
		);
}

