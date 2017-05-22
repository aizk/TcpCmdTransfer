#ifndef TCPCMDRECV_H
#define TCPCMDRECV_H

#include "cmdrecv.h"
#include <QtCore>

class TcpServer;
class TcpSocket;

class TcpCmdRecv : public CmdRecv
{
public:
	TcpCmdRecv(CmdFactory *pCmdFactory);
	//void handleCmdReady(quint32 devId, TcpSocket * tcpSocket, QByteArray data);
    virtual void handleCmd(quint32 devId, TcpSocket * tcpSocket, QByteArray data) = 0;
	bool hasCmdData();
	void createCmd(); 

protected:
    TcpServer *m_pTcpServer;
    quint16 m_listenPort;
    
};

#endif // TCPCMDRECV_H
