#ifndef TCPDEVCMDRECV_H
#define TCPDEVCMDRECV_H

#include "tcpcmdrecv.h"

class TcpDevCmdRecv : public TcpCmdRecv
{
public:
	TcpDevCmdRecv(CmdFactory *pCmdFactory);

    void handleCmd(quint32 devId, TcpSocket * tcpSocket, QByteArray data);
};

#endif // TCPDEVCMDRECV_H
