#include "udpcmdrecv.h"

UDPCmdRecv::UDPCmdRecv(CmdFactory *pCmdFactory, QUdpSocket *pUdpSocket)
    : CmdRecv(pCmdFactory),
      m_pUdpSocket(pUdpSocket)
{

}

bool UDPCmdRecv::hasCmdData()
{
    return m_pUdpSocket->hasPendingDatagrams();
}

