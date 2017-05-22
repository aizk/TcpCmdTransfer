#ifndef DEVCMDRECV_H
#define DEVCMDRECV_H

#include "udpcmdrecv.h"

class DevCmdRecv : public UDPCmdRecv
{

public:
	DevCmdRecv(CmdFactory *pCmdFactory, QUdpSocket *pUdpSocket);
    void createCmd();
};

#endif // DEVCMDRECV_H
