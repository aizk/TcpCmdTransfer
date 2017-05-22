/********************************************************************
	创建时间:	2016/8/9 15:14
	文件名: 	cmdfactory.cpp
	作者:		余谨智
	目的:	    创建命令对象的工厂。是一个抽象类，子类需要实现createDevOrAppHandler
				方法，和updateDevOrAppHandler方法。
*********************************************************************/
#include "cmdfactory.h"
#include "nochangetranslator.h"
#include "platformtranslator.h"
#include "replytodevtranslator.h"
#include "cmd.h"
#include "alarmdev.h"
#include "utils.h"
#include "const.h"
#include "crc16.h"
#include "cmdrunnable.h"

QReadWriteLock CmdFactory::m_lockMapDevIdToDevHandler;
QReadWriteLock CmdFactory::m_lockMapDevIdToAppHandler;

CmdFactory::CmdFactory()
    : m_pDevToAppTranslator(NULL),
	  m_pAppToDevTranslator(NULL),
	  m_pDevToPlatformTranslator(NULL),
	  m_pPlatformToDevTranslator(NULL)
{
    m_pDevToAppTranslator = new NoChangeTranslator(CmdTranslator::FROM_DEV);
    m_pAppToDevTranslator = new NoChangeTranslator(CmdTranslator::TO_DEV);
    m_pDevToPlatformTranslator = new PlatformTranslator(CmdTranslator::FROM_DEV);
	m_pPlatformToDevTranslator = new PlatformTranslator(CmdTranslator::TO_DEV);
	m_pReplyToDevTranslator = new ReplyToDevTranslator();
}

CmdFactory::~CmdFactory()
{
	qDeleteAll(m_mapDevIdToDevHandler.values());
	qDeleteAll(m_mapDevIdToAppHandler.values());
	m_mapDevIdToDevHandler.clear();
	m_mapDevIdToAppHandler.clear();
    delete m_pDevToAppTranslator;
	delete m_pAppToDevTranslator;
	delete m_pDevToPlatformTranslator;
	delete m_pPlatformToDevTranslator;
	delete m_pReplyToDevTranslator;
}

//************************************
    // 方法名称:    createFromDevCmd
    // 全名:        CmdFactory::createFromDevCmd
    // 可访问性:    public 
    // 返回值:      void
    // 限定符:     
    // 参数:        const quint32 devId					设备ID
    // 参数:        const QByteArray & recvCmd			接收到的指令帧
    // 参数:        const QHostAddress & senderAddress	指令发送方IP地址
    // 参数:        quint16 senderPort					指令发送方端口号
    // 说明: 		创建来自设备的命令
    //       
    //************************************
void CmdFactory::createFromDevCmd(
        const quint32 devId,
        const QByteArray &recvCmd,
        const QHostAddress &senderAddress,
        quint16 senderPort
        )
{
	Utils::logCmd(Const::FROM_DEV, recvCmd);
	QDataStream is(recvCmd);
	is.skipRawData(5);
	quint16 cmdId = 0;
	quint16 mid = 0;
	is >> cmdId >> mid; // 读取指令帧中的命令ID及MID
	m_lockMapDevIdToDevHandler.lockForRead();
	if(!m_mapDevIdToDevHandler.contains(devId))
	{
		// 如果m_mapDevIdToDevHandler中还没有设备ID为devId的设备处理器，
		// 则创建新的报警设备
		AlarmDev *pAlarmDev = createAlarmDev(devId, senderAddress, senderPort);
		if(!pAlarmDev)
			return;
		// 创建设备上线命令
		createDevOnlineCmd(pAlarmDev);
		// 创建新的设备处理器
		createDevOrAppHandler(devId, senderAddress, senderPort, true);
		
	}
	else
	{
		// 如果m_mapDevIdToDevHandler已存在设备ID为devId的设备处理器,
		// 则更新设备处理器
		updateDevOrAppHandler(devId, senderAddress, senderPort, true);
	}
	m_lockMapDevIdToDevHandler.unlock();
	// 根据设备ID获取设备对象
	AlarmDev *pAlarmDev = AlarmDev::getAlarmDevByDevId(devId);
	if(!pAlarmDev)
		return;
	if(Const::MID_TO_APP != mid 
		&& 
		(Utils::isAllBufangRetCmdId(cmdId) 
		|| Utils::isAllCFangRetCmdId(cmdId))
		)
	{
		// 只有平台下发的命令才调用endCmdExecing接口来设置命令执行完毕标识
		if(pAlarmDev)
		{
			pAlarmDev->endCmdExecing();
		}
	}
	if(Utils::isHeartbeatCmdId(cmdId))
	{
		// 如果是心跳指令，则设置设备心跳
		pAlarmDev->setHeartbeat();
	}
	if(Utils::isRecvDataError(cmdId))
	{
		// 如果是设备指令接收错误回复，则立即返回
		return;
	}
	// 根据接收到的设备ID，命令ID, MID，接收到的指令帧来生成命令对象
	Cmd *pCmd = createCmdObjFromDev(devId, cmdId, mid, recvCmd);
	if(!pCmd)
		return;
	runInThreadPool(pCmd);
}

//************************************
    // 方法名称:    createFromAppCmd
    // 全名:        CmdFactory::createFromPhoneCmd
    // 可访问性:    public 
    // 返回值:      void
    // 限定符:     
	// 参数:        const quint32 devId					设备ID
	// 参数:        const QByteArray & recvCmd			接收到的指令帧
	// 参数:        const QHostAddress & senderAddress	指令发送方IP地址
	// 参数:        quint16 senderPort					指令发送方端口号
    // 说明: 		创建来自APP的命令
    //       
    //************************************
void CmdFactory::createFromAppCmd(
        const quint32 devId,
        const QByteArray &recvCmd,
        const QHostAddress &senderAddress,
        quint16 senderPort
        )
{
	Utils::logCmd(Const::FROM_APP, recvCmd);
	QDataStream is(recvCmd);
	is.skipRawData(5);
	quint16 cmdId = 0;
	quint16 mid = 0;
	is >> cmdId >> mid;
	if(!m_mapDevIdToAppHandler.contains(devId))
	{
		createDevOrAppHandler(devId, senderAddress, senderPort, false);
	}
	else
	{
		updateDevOrAppHandler(devId, senderAddress, senderPort, false);
	}
	if(Utils::isAppHeart(cmdId))
	{
		ICmdHandler *pToPhoneHandler = m_mapDevIdToAppHandler.value(devId);
		createAppReplyCmd(devId, mid, pToPhoneHandler);
		return;
	}
    ICmdHandler *pToDevHandler = m_mapDevIdToDevHandler.value(devId);
	if(!pToDevHandler)
		return;
	ListPCmdHandler lstPCmdHandler;
	lstPCmdHandler.append(pToDevHandler);
    Cmd *pCmd = new Cmd(recvCmd);
    pCmd->addTranslatorAndHandlers(m_pAppToDevTranslator, lstPCmdHandler);
	runInThreadPool(pCmd);
}

//************************************
	// 方法名称:    createDevOnlineCmd
	// 全名:        CmdFactory::createDevOnlineCmd
	// 可访问性:    public 
	// 返回值:      void
	// 限定符:     
	// 参数:        AlarmDev * const pAlarmDev	刚上线的设备对象
	// 说明: 		创建设备上线命令
	//       
	//************************************
void CmdFactory::createDevOnlineCmd( AlarmDev * const pAlarmDev )
{
	QByteArray aCmd;
	QDataStream os(&aCmd, QIODevice::WriteOnly);
	quint8 devType = 0;
	quint32 devId = pAlarmDev->devId;
	quint16 cmdId = Const::CMD_ID_ONLINE;
	quint16 mid = 0;
	quint16 dataLen = 1;
	quint8 online = pAlarmDev->onlineStatus ? 1 : 0;
	os << devType << devId << cmdId << mid << dataLen << online;
	ListPCmdHandler lstPlatformHandler;
	lstPlatformHandler.append(HTTPHandler::getHttpHandler());
	Cmd *pCmd = new Cmd(aCmd);
	pCmd->addTranslatorAndHandlers(m_pDevToPlatformTranslator, lstPlatformHandler);
	runInThreadPool(pCmd);
}

//************************************
// 方法名称:    createAlarmDev
// 全名:        CmdFactory::createAlarmDev
// 可访问性:    protected 
// 返回值:      AlarmDev *
// 限定符:     
// 参数:        const quint32 devId			设备ID
// 参数:        const QHostAddress & addr	设备IP地址
// 参数:        const quint16 port			设备端口号
// 说明: 		创建报警设备对象
//       
//************************************
AlarmDev * CmdFactory::createAlarmDev( const quint32 devId, const QHostAddress &addr, const quint16 port )
{
	AlarmDev *pAlarmDev = new AlarmDev();
	pAlarmDev->devId = devId;
	pAlarmDev->hostAddr = addr;
	pAlarmDev->port = port;
	AlarmDev::insertAlarmDevToMap(pAlarmDev);
	return pAlarmDev;
}

//************************************
// 方法名称:    createCmdObjFromDev
// 全名:        CmdFactory::createCmdObjFromDev
// 可访问性:    virtual protected 
// 返回值:      Cmd *
// 限定符:     
// 参数:        const quint32 devId			设备ID
// 参数:        const quint16 cmdId			命令ID
// 参数:        const quint16 mid			MID
// 参数:        const QByteArray & recvCmd	接收到的指令帧
// 说明: 		创建来自设备的命令对象，即组装翻译器及处理器。
//       
//************************************
Cmd * CmdFactory::createCmdObjFromDev( const quint32 devId, const quint16 cmdId, const quint16 mid, const QByteArray & recvCmd )
{
	ListPCmdHandler lstPlatformHandler;
	ListPCmdHandler lstToPhoneUdpHandler;
	ListPCmdHandler lstDevReplyHandler;
	Cmd *pCmd = new Cmd(recvCmd);
	ICmdHandler *pDevHandler = m_mapDevIdToDevHandler[devId];
	lstDevReplyHandler.append(pDevHandler);
	pCmd->addTranslatorAndHandlers(m_pReplyToDevTranslator, lstDevReplyHandler);

	lstToPhoneUdpHandler = m_mapDevIdToAppHandler.values(devId);
	if(lstToPhoneUdpHandler.size() != 0 
		&& 
		(   Utils::isAlarmReportCmdId(cmdId)
		 || Utils::isHeartbeatCmdId(cmdId)
		 || Const::MID_TO_APP == mid
		 )
	  )
	{
		pCmd->addTranslatorAndHandlers(m_pDevToAppTranslator, lstToPhoneUdpHandler);
	}
	if(	
			Utils::isAlarmReportCmdId(cmdId)
		 || Utils::isHeartbeatCmdId(cmdId)
		 || (Const::MID_TO_APP != mid 
			&& (Utils::isAllBufangRetCmdId(cmdId)
			  ||Utils::isAllCFangRetCmdId(cmdId)))
	  )
	{
		lstPlatformHandler.append(HTTPHandler::getHttpHandler());
		pCmd->addTranslatorAndHandlers(m_pDevToPlatformTranslator, lstPlatformHandler);
	}
	
	return pCmd;
}

//************************************
// 方法名称:    createAppReplyCmd
// 全名:        CmdFactory::createAppReplyCmd
// 可访问性:    virtual protected 
// 返回值:      void
// 限定符:     
// 参数:        const quint32 iDevId	
// 参数:        const quint16 iMid	
// 参数:        ICmdHandler * pAppHandler	
// 说明: 		创建APP回复命令
//       
//************************************
void CmdFactory::createAppReplyCmd( const quint32 iDevId, const quint16 iMid, ICmdHandler *pAppHandler )
{
	QByteArray aCmd;
	QDataStream os(&aCmd, QIODevice::WriteOnly);
	quint8 devType = Const::DEVTYPE_BOARD;
	quint32 devId = iDevId;
	quint16 cmdId = Const::CMD_ID_APP_HEART_REPLY;
	quint16 mid = iMid;
	quint16 dataLen = 0;
	os	<< devType 
		<< devId 
		<< cmdId 
		<< mid 
		<< dataLen;
	quint16 crc = Utils::modbus_crc((quint8 *)aCmd.data(), aCmd.size());
	os << crc;
	ListPCmdHandler lstPhoneHandler;
	lstPhoneHandler.append(pAppHandler);
	Cmd *pCmd = new Cmd(aCmd);
	pCmd->addTranslatorAndHandlers(m_pDevToAppTranslator, lstPhoneHandler);
	runInThreadPool(pCmd);
}

//************************************
// 方法名称:    runInThreadPool
// 全名:        CmdFactory::runInThreadPool
// 可访问性:    virtual protected 
// 返回值:      void
// 限定符:     
// 参数:        Cmd * pCmd	命令对象指针
// 说明: 		在全局线程池中运行命令
//       
//************************************
void CmdFactory::runInThreadPool( Cmd *pCmd )
{
	CmdRunnable *pCmdRunnable = new CmdRunnable(pCmd);
	QThreadPool::globalInstance()->start(pCmdRunnable);
}

//************************************
// 方法名称:    createFromPlatformCmd
// 全名:        CmdFactory::createFromPlatformCmd
// 可访问性:    public 
// 返回值:      void
// 限定符:     
// 参数:        const quint32 devId	
// 参数:        const QByteArray & recvCmd	
// 参数:        const QHostAddress & senderAddress	
// 参数:        quint16 senderPort	
// 说明: 		创建来自平台的命令对象
//       
//************************************
void CmdFactory::createFromPlatformCmd( const quint32 devId, const QByteArray &recvCmd, const QHostAddress &senderAddress, quint16 senderPort )
{
	// TCP版createFromPlatformCmd未实现
	return;
}

