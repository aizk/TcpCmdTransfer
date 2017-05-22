/********************************************************************
	����ʱ��:	2016/8/9 15:35
	�ļ���: 	tcpcmdfactory.cpp
	����:		�����
	Ŀ��:	    ����TCPЭ�鴫���豸ָ������������createDevOrAppHandler��updateDevOrAppHandler
				�д�����ά��TcpHandler��
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
// ��������:    createDevOrAppHandler
// ȫ��:        TcpCmdFactory::createDevOrAppHandler
// �ɷ�����:    protected 
// ����ֵ:      void
// �޶���:     
// ����:        quint32 devId					�豸ID
// ����:        const QHostAddress & senderAddr	������IP��ַ
// ����:        const quint16 senderPort		������port
// ����:        bool isDev						���豸������APP
// ˵��: 		�����µ�TcpHandler�����Ž�m_mapDevIdToDevHandler��m_mapDevIdToAppHandler
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
// ��������:    updateDevOrAppHandler
// ȫ��:        TcpCmdFactory::updateDevOrAppHandler
// �ɷ�����:    protected 
// ����ֵ:      void
// �޶���:     
// ����:        quint32 devId					�豸ID
// ����:        const QHostAddress & senderAddr	������IP��ַ
// ����:        const quint16 senderPort		������port
// ����:        bool isDev						���豸������APP
// ˵��: 		����TcpHandler�������豸IDΪdevId���豸��IP��ַ��˿ںŸı��ˣ���ô����Ҫ
//				�������Ӧ��TcpHandler
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
// ��������:    handleSocketDisconnect
// ȫ��:        TcpCmdFactory::handleSocketDisconnect
// �ɷ�����:    public 
// ����ֵ:      void
// �޶���:     
// ����:        quint32 devId			�豸ID
// ����:        TcpSocket * pTcpSocket	�Ͽ����ӵ�TcpSocket
// ˵��: 		������Ͽ�TcpSocket��صĶ���
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

