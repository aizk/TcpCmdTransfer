/********************************************************************
	创建时间:	2016/8/9 15:16
	文件名: 	tcpcmdfactory.h
	作者:		余谨智
	目的:	    运用TCP协议传送设备指令的命令工厂，在createDevOrAppHandler和updateDevOrAppHandler
				中创建和维护TcpHandler。
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
	QMap<TcpSocket *, TcpHandler *> m_mapTcpSocketToHandler; // 通过TcpSocket找相应的TcpHandler
	static QReadWriteLock m_mapLock;
};

#endif // TCPCMDFACTORY_H
