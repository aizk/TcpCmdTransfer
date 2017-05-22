#include "devcmdrecv.h"
#include "utils.h"

static const int MAX_DEV_CMD_SIZE = 300;
static char devCmdBuffer[MAX_DEV_CMD_SIZE];

DevCmdRecv::DevCmdRecv( CmdFactory *pCmdFactory, QUdpSocket *pUdpSocket ) : UDPCmdRecv(pCmdFactory, pUdpSocket)
{

}

void DevCmdRecv::createCmd()
{
    QHostAddress senderAddr;
    quint16 senderPort = 0;
    memset(devCmdBuffer, 0, MAX_DEV_CMD_SIZE);
    qint64 bytesRead = m_pUdpSocket->readDatagram(devCmdBuffer, MAX_DEV_CMD_SIZE, &senderAddr, &senderPort);
	if(bytesRead <= 0)
		return;
    QByteArray aDevCmd(devCmdBuffer, bytesRead);
    quint32 devId = Utils::getDevIdFromCmd(aDevCmd);
    m_pCmdFactory->createFromDevCmd(devId, aDevCmd, senderAddr, senderPort);
}

