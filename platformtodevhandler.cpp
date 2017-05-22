#include "platformtodevhandler.h"
#include "utils.h"
#include "platformtranslator.h"
#include "alarmdev.h"
#include "const.h"
#include "crc16.h"
#include <iostream>

static const int WAIT_TIME_FOR_DEV_RET = 5000; // 5s

PlatformToDevHandler::PlatformToDevHandler()
    : m_platformRecvSocket(0),
      m_toDevSocket(0)
{
}

int PlatformToDevHandler::handleCmd(QByteArray &aCmd)
{
    QDataStream is(aCmd);
    is.skipRawData(1);
    quint32 devId = 0;
    quint16 cmdId = 0;
    is >> devId >> cmdId;
    AlarmDev *pAlarmDev = AlarmDev::getAlarmDevByDevId(devId);
	QJsonObject retJsonObj;
	int retCode = Const::CMD_DEV_OFFLINE;
    if(Utils::isAllBufangCmdId(cmdId) || Utils::isAllCFangCmdId(cmdId))
    {
        if(!pAlarmDev)
        {
            retCode = Const::CMD_DEV_NOT_EXIST;
        }
        else if(false == pAlarmDev->onlineStatus)
        {
            retCode = Const::CMD_DEV_OFFLINE;
        }
		else
		{
			if(m_toDevSocket)
			{
				Utils::logCmd(Const::REPLY_TO_DEV, aCmd);
				m_toDevSocket->writeDatagram(aCmd, pAlarmDev->hostAddr, pAlarmDev->port);
				pAlarmDev->startCmdTiming();
				while(!pAlarmDev->isCmdTimeout() && pAlarmDev->isCmdExecing());
				if(pAlarmDev->isCmdTimeout())
				{
					retCode = Const::CMD_TIMEOUT;
				}
				else
				{
					retCode = Const::CMD_SECCESS;
				}
				pAlarmDev->endCmdExecing();
			}
		}
		if(Utils::isAllBufangCmdId(cmdId))
			cmdId = Const::CMD_ID_ALL_BUFANG_RTN;
		else if(Utils::isAllCFangCmdId(cmdId))
			cmdId = Const::CMD_ID_ALL_CFANG_RTN;
		jsonCmdRetToPlatform(retJsonObj, devId, cmdId, retCode);
    }
    
    
    QJsonDocument retJsonDoc(retJsonObj);
    m_platformRecvSocket->writeDatagram(retJsonDoc.toJson(QJsonDocument::Compact), *m_platformHostAddr, m_platformPort);
    return 0;
}

void PlatformToDevHandler::setPlatformHostAddr( const QHostAddress &platformHostAddr )
{
    m_platformHostAddr = new QHostAddress(platformHostAddr);
}

void PlatformToDevHandler::setToDevSocket(QUdpSocket *pToDevSocket)
{
    m_toDevSocket = pToDevSocket;
}

void PlatformToDevHandler::setPlatformRecvSocket(QUdpSocket *pPlatformRecvSocket)
{
    m_platformRecvSocket = pPlatformRecvSocket;
}

void PlatformToDevHandler::setPlatformPort(const quint16 port)
{
    m_platformPort = port;
}

PlatformToDevHandler::~PlatformToDevHandler()
{
	delete m_platformHostAddr;
}



