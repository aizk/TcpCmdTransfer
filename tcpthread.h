#ifndef TCPTHREAD_H
#define TCPTHREAD_H

#include <QtNetwork>
#include "tcpcmdfactory.h"
#include "tcpcmdrecv.h"
#include "tcpsocket.h"

class TcpThread : public QThread
{
    Q_OBJECT
public:
    TcpThread();
    ~TcpThread();
    void setTcpCmdFactory(TcpCmdFactory *pTcpCmdFactory);
    TcpSocket *getTcpSocketByDevId(const quint32 devId);
    void setTcpCmdRecv(TcpCmdRecv * pTcpCmdRecv);
	void setIsDev(bool isDev);
	bool isDev();

public slots:
    void handleNewSocketDescriptor(qintptr socketDescriptor);

protected slots:
    void handleCmdReady(quint32 devId, TcpSocket *pTcpSocket, QByteArray data);
	void handleSocketDisconnect(quint32 devId, TcpSocket *pTcpSocket);
	void handleToDisconnect(TcpSocket *pTcpSocket);

protected:
    QList<qintptr> m_lstSocketDescriptor;
    QList<TcpSocket *> m_lstTcpSocket;
    QMap<quint32, TcpSocket *> m_mapDevIdToTcpSocket;
    TcpCmdFactory *m_pTcpCmdFactory;
    TcpCmdRecv *m_pTcpCmdRecv;
	bool m_isDev;
};

#endif // TCPTHREAD_H
