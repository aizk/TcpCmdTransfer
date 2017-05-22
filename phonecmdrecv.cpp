#include "phonecmdrecv.h"
#include "utils.h"

static const int MAX_PHONE_CMD_SIZE = 300;
static char phoneCmdBuffer[MAX_PHONE_CMD_SIZE];

PhoneCmdRecv::PhoneCmdRecv( CmdFactory *pCmdFactory, QUdpSocket *pUdpSocket ) : UDPCmdRecv(pCmdFactory, pUdpSocket)
{

}

void PhoneCmdRecv::createCmd()
{
    QHostAddress senderAddr;
    quint16 senderPort = 0;
    memset(phoneCmdBuffer, 0, MAX_PHONE_CMD_SIZE);
    qint64 bytesRead = m_pUdpSocket->readDatagram(phoneCmdBuffer, MAX_PHONE_CMD_SIZE, &senderAddr, &senderPort);
	if(bytesRead <= 0)
		return;
	QByteArray aPhoneCmd(phoneCmdBuffer, bytesRead);
    quint32 devId = Utils::getDevIdFromCmd(aPhoneCmd);
    m_pCmdFactory->createFromAppCmd(devId, aPhoneCmd, senderAddr, senderPort);
}

