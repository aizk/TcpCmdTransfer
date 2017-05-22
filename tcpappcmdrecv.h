#ifndef TCPAPPCMDRECV_H
#define TCPAPPCMDRECV_H

#include "tcpcmdrecv.h"

class TcpAppCmdRecv : public TcpCmdRecv
{
public:
	TcpAppCmdRecv(CmdFactory *pCmdFactory);

    void handleCmd(quint32 devId, TcpSocket * tcpSocket, QByteArray data);
};

#endif // TCPAPPCMDRECV_H
