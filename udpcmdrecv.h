#ifndef UDPCMDRECV_H
#define UDPCMDRECV_H

#include "cmdrecv.h"
#include <QtNetwork>

class UDPCmdRecv : public CmdRecv
{
public:
    UDPCmdRecv(CmdFactory *pCmdFactory, QUdpSocket *pUdpSocket);
    bool hasCmdData();

protected:
    QUdpSocket *m_pUdpSocket;
};

#endif // UDPCMDRECV_H
