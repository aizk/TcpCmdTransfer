/********************************************************************
	创建时间:	2016/8/9 15:02
	文件名: 	cmdfactory.h
	作者:		余谨智
	目的:	    创建命令对象的工厂。是一个抽象类，子类需要实现createDevOrAppHandler
				方法，和updateDevOrAppHandler方法。
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
    PCmdTranslator m_pDevToAppTranslator;		// 设备到App的翻译器
    PCmdTranslator m_pAppToDevTranslator;		// APP到设备的翻译器
    PCmdTranslator m_pDevToPlatformTranslator;	// 设备到平台的翻译器
	PCmdTranslator m_pPlatformToDevTranslator;	// 平台到设备的翻译器
	PCmdTranslator m_pReplyToDevTranslator;		// 回复设备的翻译器
	
    QMap<quint32, ICmdHandler *> m_mapDevIdToDevHandler;		// 设备ID到设备处理器的map
    QMultiMap<quint32, ICmdHandler *> m_mapDevIdToAppHandler;	// 设备ID到APP处理器的map
	static QReadWriteLock m_lockMapDevIdToDevHandler;			// m_mapDevIdToDevHandler的锁
	static QReadWriteLock m_lockMapDevIdToAppHandler;			// m_mapDevIdToAppHandler的锁
};

#endif // CMDFACTORY_H
