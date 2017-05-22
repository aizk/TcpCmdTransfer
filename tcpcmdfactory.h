/********************************************************************
	����ʱ��:	2016/8/9 15:16
	�ļ���: 	tcpcmdfactory.h
	����:		�����
	Ŀ��:	    ����TCPЭ�鴫���豸ָ������������createDevOrAppHandler��updateDevOrAppHandler
				�д�����ά��TcpHandler��
*********************************************************************/
#ifndef TCPCMDFACTORY_H
#define TCPCMDFACTORY_H

#include "cmdfactory.h"
#include "tcpserver.h"
#include "tcpsocket.h"
#include "tcphandler.h"

class TcpCmdFactory : public CmdFactory, public QObject
{
public:
    TcpCmdFactory();
    ~TcpCmdFactory();

public slots:
	void handleSocketDisconnect(quint32 devId, TcpSocket *pTcpSocket);

protected:
	void createDevOrAppHandler(quint32 devId, const QHostAddress &senderAddr, const quint16 senderPort, bool isDev);
	void updateDevOrAppHandler(quint32 devId, const QHostAddress &senderAddr, const quint16 senderPort, bool isDev);

protected:
	QMap<TcpSocket *, TcpHandler *> m_mapTcpSocketToHandler; // ͨ��TcpSocket����Ӧ��TcpHandler
	static QReadWriteLock m_mapLock;
};

#endif // TCPCMDFACTORY_H
