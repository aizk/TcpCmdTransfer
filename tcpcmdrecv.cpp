#include "tcpcmdrecv.h"
#include "tcpserver.h"

TcpCmdRecv::TcpCmdRecv(CmdFactory *pCmdFactory)
	:	CmdRecv(pCmdFactory)
{
}

bool TcpCmdRecv::hasCmdData()
{
	return true;
}

void TcpCmdRecv::createCmd()
{
	return;
}

