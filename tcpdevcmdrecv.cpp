#include "tcpdevcmdrecv.h"
#include "utils.h"
#include "tcpsocket.h"

TcpDevCmdRecv::TcpDevCmdRecv( CmdFactory *pCmdFactory ) : TcpCmdRecv(pCmdFactory)
{

}

void TcpDevCmdRecv::handleCmd(quint32 devId, TcpSocket * tcpSocket, QByteArray data )
{
	m_pCmdFactory->createFromDevCmd(
		devId, 
		data, 
		tcpSocket->localAddress(), 
		tcpSocket->localPort()
		);
}

