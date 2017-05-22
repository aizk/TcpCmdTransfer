/********************************************************************
	创建时间:	2016/8/9 15:35
	文件名: 	tcpcmdfactory.cpp
	作者:		余谨智
	目的:	    运用TCP协议传送设备指令的命令工厂，在createDevOrAppHandler和updateDevOrAppHandler
				中创建和维护TcpHandler。
*********************************************************************/
#include "tcpcmdfactory.h"
#include "const.h"
#include "utils.h"
#include "tcpthread.h"

QReadWriteLock TcpCmdFactory::m_mapLock;

TcpCmdFactory::TcpCmdFactory()
{

}

TcpCmdFactory::~TcpCmdFactory()
{

}

//************************************
// 方法名称:    createDevOrAppHandler
// 全名:        TcpCmdFactory::createDevOrAppHandler
// 可访问性:    protected 
// 返回值:      void
// 限定符:     
// 参数:        quint32 devId					设备ID
// 参数:        const QHostAddress & senderAddr	发送者IP地址
// 参数:        const quint16 senderPort		发送者port
// 参数:        bool isDev						是设备，还是APP
// 说明: 		创建新的TcpHandler，并放进m_mapDevIdToDevHandler或m_mapDevIdToAppHandler
//       
//************************************
void TcpCmdFactory::createDevOrAppHandler( quint32 devId, const QHostAddress &senderAddr, const quint16 senderPort, bool isDev)
{
	QWriteLocker locker(&m_mapLock);
    QThread *pThread = QThread::currentThread();
    TcpThread *pTcpThread = static_cast<TcpThread *>(pThread);
    TcpSocket *pTcpSocket = pTcpThread->getTcpSocketByDevId(devId);
	if(!pTcpSocket)
		return;
	TcpHandler *pToTcpHandler = new TcpHandler(pTcpSocket);
	pToTcpHandler->setValid(true);
	if(isDev)
	{
		m_mapDevIdToDevHandler.insert(devId, pToTcpHandler);
	}
	else
	{
		m_mapDevIdToAppHandler.insert(devId, pToTcpHandler);
	}
	m_mapTcpSocketToHandler.insert(pTcpSocket, pToTcpHandler);
}

//************************************
// 方法名称:    updateDevOrAppHandler
// 全名:        TcpCmdFactory::updateDevOrAppHandler
// 可访问性:    protected 
// 返回值:      void
// 限定符:     
// 参数:        quint32 devId					设备ID
// 参数:        const QHostAddress & senderAddr	发送者IP地址
// 参数:        const quint16 senderPort		发送者port
// 参数:        bool isDev						是设备，还是APP
// 说明: 		更新TcpHandler，比如设备ID为devId的设备的IP地址或端口号改变了，那么就需要
//				更新相对应的TcpHandler
//************************************
void TcpCmdFactory::updateDevOrAppHandler( quint32 devId, const QHostAddress &senderAddr, const quint16 senderPort, bool isDev)
{
	QReadLocker locker(&m_mapLock);
    TcpHandler *pTcpHandler = NULL;
	if(isDev)
	{
		if(m_mapDevIdToDevHandler.contains(devId))
		{
			pTcpHandler = static_cast<TcpHandler *>(m_mapDevIdToDevHandler[devId]);
		}
	}
	else
	{
		if(m_mapDevIdToAppHandler.contains(devId))
		{
			ICmdHandler *pCmdHandler = m_mapDevIdToAppHandler.values(devId).at(0);
			pTcpHandler = static_cast<TcpHandler *>(pCmdHandler);
		}
	}
	if(!pTcpHandler)
		return;
	if(pTcpHandler->isValid())
	{
		return;
	}
	QThread *pThread = QThread::currentThread();
	TcpThread *pTcpThread = static_cast<TcpThread *>(pThread);
	TcpSocket *pTcpSocket = pTcpThread->getTcpSocketByDevId(devId);
	if(pTcpSocket)
	{
		pTcpHandler->setTcpSocket(pTcpSocket);
		pTcpHandler->setValid(true);
	}
}

//************************************
// 方法名称:    handleSocketDisconnect
// 全名:        TcpCmdFactory::handleSocketDisconnect
// 可访问性:    public 
// 返回值:      void
// 限定符:     
// 参数:        quint32 devId			设备ID
// 参数:        TcpSocket * pTcpSocket	断开连接的TcpSocket
// 说明: 		清理与断开TcpSocket相关的对象。
//       
//************************************
void TcpCmdFactory::handleSocketDisconnect( quint32 devId, TcpSocket *pTcpSocket )
{
	if(0 == devId)
		return;
	AlarmDev::delAlarmDevByDevId(devId);
	m_mapLock.lockForRead();
	TcpHandler *pTcpHandler = m_mapTcpSocketToHandler[pTcpSocket];
	m_mapLock.unlock();
	if(pTcpHandler)
	{
		m_mapLock.lockForWrite();
		if(pTcpSocket->isDev())
		{
			m_mapDevIdToDevHandler.remove(devId);
		}
		else
		{
			m_mapDevIdToAppHandler.remove(devId);
		}
		m_mapTcpSocketToHandler.remove(pTcpSocket);
		pTcpHandler->deleteLater();
		m_mapLock.unlock();
	}
}

