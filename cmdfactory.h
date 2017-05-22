/********************************************************************
	����ʱ��:	2016/8/9 15:02
	�ļ���: 	cmdfactory.h
	����:		�����
	Ŀ��:	    �����������Ĺ�������һ�������࣬������Ҫʵ��createDevOrAppHandler
				��������updateDevOrAppHandler������
*********************************************************************/
#ifndef CMDFACTORY_H
#define CMDFACTORY_H

#include "icmdtranslator.h"
#include "httphandler.h"
#include "platformtodevhandler.h"
#include "alarmdev.h"
#include "cmd.h"

class CmdFactory
{
public:
    CmdFactory();
    ~CmdFactory();

    void createFromDevCmd(
			const quint32 devId,
            const QByteArray &recvCmd,
            const QHostAddress &senderAddress,
            quint16 senderPort
            );
    void createFromAppCmd(
			const quint32 devId,
            const QByteArray &recvCmd,
            const QHostAddress &senderAddress,
            quint16 senderPort
            );
	void createFromPlatformCmd(
			const quint32 devId,
			const QByteArray &recvCmd,
			const QHostAddress &senderAddress,
			quint16 senderPort
			);
    void createDevOnlineCmd(AlarmDev * const pAlarmDev);

protected:
	AlarmDev *createAlarmDev(const quint32 devId, const QHostAddress &addr, const quint16 port);
    virtual Cmd *createCmdObjFromDev(const quint32 devId, const quint16 cmdId, const quint16 mid, const  QByteArray & recvCmd);
	virtual void createAppReplyCmd(const quint32 iDevId, const quint16 iMid, ICmdHandler *pPhoneHandler);
	virtual void createDevOrAppHandler(quint32 devId, const QHostAddress &senderAddr, const quint16 senderPort, bool isDev) = 0;
	virtual void updateDevOrAppHandler(quint32 devId, const QHostAddress &senderAddr, const quint16 senderPort, bool isDev) = 0; 
	virtual void runInThreadPool(Cmd *pCmd);

protected:
    PCmdTranslator m_pDevToAppTranslator;		// �豸��App�ķ�����
    PCmdTranslator m_pAppToDevTranslator;		// APP���豸�ķ�����
    PCmdTranslator m_pDevToPlatformTranslator;	// �豸��ƽ̨�ķ�����
	PCmdTranslator m_pPlatformToDevTranslator;	// ƽ̨���豸�ķ�����
	PCmdTranslator m_pReplyToDevTranslator;		// �ظ��豸�ķ�����
	
    QMap<quint32, ICmdHandler *> m_mapDevIdToDevHandler;		// �豸ID���豸��������map
    QMultiMap<quint32, ICmdHandler *> m_mapDevIdToAppHandler;	// �豸ID��APP��������map
	static QReadWriteLock m_lockMapDevIdToDevHandler;			// m_mapDevIdToDevHandler����
	static QReadWriteLock m_lockMapDevIdToAppHandler;			// m_mapDevIdToAppHandler����
};

#endif // CMDFACTORY_H
