/********************************************************************
	创建时间:	2016/8/9 17:11
	文件名: 	platformcmdrecv.cpp
	作者:		余谨智
	目的:	    平台命令接收器
*********************************************************************/
#include "platformcmdrecv.h"
#include "utils.h"

static const int MAX_PLATFORM_CMD_SIZE = 1024;
static char platformCmdBuffer[MAX_PLATFORM_CMD_SIZE];

PlatformCmdRecv::PlatformCmdRecv(CmdFactory *pCmdFactory, QUdpSocket *pUdpSocket)
    : UDPCmdRecv(pCmdFactory, pUdpSocket)
{

}

void PlatformCmdRecv::createCmd()
{
    QHostAddress senderAddr;
    quint16 senderPort = 0;
    memset(platformCmdBuffer, 0, MAX_PLATFORM_CMD_SIZE);
    qint64 bytesRead = m_pUdpSocket->readDatagram(platformCmdBuffer, MAX_PLATFORM_CMD_SIZE, &senderAddr, &senderPort);
	if(bytesRead <= 0)
		return;
    QByteArray aPlatformCmd(platformCmdBuffer, bytesRead);
	QJsonParseError jsonError;
	QJsonDocument::fromJson(aPlatformCmd, &jsonError);
	if(jsonError.error != QJsonParseError::NoError)
		return;
	quint32 devId = Utils::getDevIdFromJsonCmd(aPlatformCmd);
    //m_pCmdFactory->createFromPlatformCmd(devId, aPlatformCmd, senderAddr, senderPort);
}

