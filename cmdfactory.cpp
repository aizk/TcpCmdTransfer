/********************************************************************
	����ʱ��:	2016/8/9 15:14
	�ļ���: 	cmdfactory.cpp
	����:		�����
	Ŀ��:	    �����������Ĺ�������һ�������࣬������Ҫʵ��createDevOrAppHandler
				��������updateDevOrAppHandler������
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
    // ��������:    createFromDevCmd
    // ȫ��:        CmdFactory::createFromDevCmd
    // �ɷ�����:    public 
    // ����ֵ:      void
    // �޶���:     
    // ����:        const quint32 devId					�豸ID
    // ����:        const QByteArray & recvCmd			���յ���ָ��֡
    // ����:        const QHostAddress & senderAddress	ָ��ͷ�IP��ַ
    // ����:        quint16 senderPort					ָ��ͷ��˿ں�
    // ˵��: 		���������豸������
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
	is >> cmdId >> mid; // ��ȡָ��֡�е�����ID��MID
	m_lockMapDevIdToDevHandler.lockForRead();
	if(!m_mapDevIdToDevHandler.contains(devId))
	{
		// ���m_mapDevIdToDevHandler�л�û���豸IDΪdevId���豸��������
		// �򴴽��µı����豸
		AlarmDev *pAlarmDev = createAlarmDev(devId, senderAddress, senderPort);
		if(!pAlarmDev)
			return;
		// �����豸��������
		createDevOnlineCmd(pAlarmDev);
		// �����µ��豸������
		createDevOrAppHandler(devId, senderAddress, senderPort, true);
		
	}
	else
	{
		// ���m_mapDevIdToDevHandler�Ѵ����豸IDΪdevId���豸������,
		// ������豸������
		updateDevOrAppHandler(devId, senderAddress, senderPort, true);
	}
	m_lockMapDevIdToDevHandler.unlock();
	// �����豸ID��ȡ�豸����
	AlarmDev *pAlarmDev = AlarmDev::getAlarmDevByDevId(devId);
	if(!pAlarmDev)
		return;
	if(Const::MID_TO_APP != mid 
		&& 
		(Utils::isAllBufangRetCmdId(cmdId) 
		|| Utils::isAllCFangRetCmdId(cmdId))
		)
	{
		// ֻ��ƽ̨�·�������ŵ���endCmdExecing�ӿ�����������ִ����ϱ�ʶ
		if(pAlarmDev)
		{
			pAlarmDev->endCmdExecing();
		}
	}
	if(Utils::isHeartbeatCmdId(cmdId))
	{
		// ���������ָ��������豸����
		pAlarmDev->setHeartbeat();
	}
	if(Utils::isRecvDataError(cmdId))
	{
		// ������豸ָ����մ���ظ�������������
		return;
	}
	// ���ݽ��յ����豸ID������ID, MID�����յ���ָ��֡�������������
	Cmd *pCmd = createCmdObjFromDev(devId, cmdId, mid, recvCmd);
	if(!pCmd)
		return;
	runInThreadPool(pCmd);
}

//************************************
    // ��������:    createFromAppCmd
    // ȫ��:        CmdFactory::createFromPhoneCmd
    // �ɷ�����:    public 
    // ����ֵ:      void
    // �޶���:     
	// ����:        const quint32 devId					�豸ID
	// ����:        const QByteArray & recvCmd			���յ���ָ��֡
	// ����:        const QHostAddress & senderAddress	ָ��ͷ�IP��ַ
	// ����:        quint16 senderPort					ָ��ͷ��˿ں�
    // ˵��: 		��������APP������
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
	// ��������:    createDevOnlineCmd
	// ȫ��:        CmdFactory::createDevOnlineCmd
	// �ɷ�����:    public 
	// ����ֵ:      void
	// �޶���:     
	// ����:        AlarmDev * const pAlarmDev	�����ߵ��豸����
	// ˵��: 		�����豸��������
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
// ��������:    createAlarmDev
// ȫ��:        CmdFactory::createAlarmDev
// �ɷ�����:    protected 
// ����ֵ:      AlarmDev *
// �޶���:     
// ����:        const quint32 devId			�豸ID
// ����:        const QHostAddress & addr	�豸IP��ַ
// ����:        const quint16 port			�豸�˿ں�
// ˵��: 		���������豸����
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
// ��������:    createCmdObjFromDev
// ȫ��:        CmdFactory::createCmdObjFromDev
// �ɷ�����:    virtual protected 
// ����ֵ:      Cmd *
// �޶���:     
// ����:        const quint32 devId			�豸ID
// ����:        const quint16 cmdId			����ID
// ����:        const quint16 mid			MID
// ����:        const QByteArray & recvCmd	���յ���ָ��֡
// ˵��: 		���������豸��������󣬼���װ����������������
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
// ��������:    createAppReplyCmd
// ȫ��:        CmdFactory::createAppReplyCmd
// �ɷ�����:    virtual protected 
// ����ֵ:      void
// �޶���:     
// ����:        const quint32 iDevId	
// ����:        const quint16 iMid	
// ����:        ICmdHandler * pAppHandler	
// ˵��: 		����APP�ظ�����
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
// ��������:    runInThreadPool
// ȫ��:        CmdFactory::runInThreadPool
// �ɷ�����:    virtual protected 
// ����ֵ:      void
// �޶���:     
// ����:        Cmd * pCmd	�������ָ��
// ˵��: 		��ȫ���̳߳�����������
//       
//************************************
void CmdFactory::runInThreadPool( Cmd *pCmd )
{
	CmdRunnable *pCmdRunnable = new CmdRunnable(pCmd);
	QThreadPool::globalInstance()->start(pCmdRunnable);
}

//************************************
// ��������:    createFromPlatformCmd
// ȫ��:        CmdFactory::createFromPlatformCmd
// �ɷ�����:    public 
// ����ֵ:      void
// �޶���:     
// ����:        const quint32 devId	
// ����:        const QByteArray & recvCmd	
// ����:        const QHostAddress & senderAddress	
// ����:        quint16 senderPort	
// ˵��: 		��������ƽ̨���������
//       
//************************************
void CmdFactory::createFromPlatformCmd( const quint32 devId, const QByteArray &recvCmd, const QHostAddress &senderAddress, quint16 senderPort )
{
	// TCP��createFromPlatformCmdδʵ��
	return;
}

