#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QtNetwork>
#include "tcpserver.h"
#include "cmdbuffer.h"

class TcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    TcpSocket();
    quint32 getDevId();
	void setIsDev(bool isDev);
	void setIsActive(bool isActive);
	bool isDev();
	bool isActive();
	static int insertTcpSocketToSet(TcpSocket *pTcpSocket);
	static void removeFromTcpSocketSet(TcpSocket *pTcpSocket);
	static void incrementAllTimer();
	void incrementNoDataTimes();
inline	void setDataTimingStart();

signals:
    void dataReady(quint32 devId, TcpSocket * pTcpSocket, QByteArray data);
	void disconnect(quint32 devId, TcpSocket * pTcpSocket);
	void toDisconnect(TcpSocket * pTcpSocket);

public slots:
    void handleReadyRead();
	void sendData(const QByteArray &dataToSend);
	void handleDisconnected();

protected:
    //int registerDevIdToTcpServer();

private:
    quint32 m_devId;
    TcpServer *m_pTcpServer;
    bool m_isDevIdGot;
    CmdBuffer m_CmdBuffer;
	bool m_isDev;
	volatile int m_noDataTimes;

	static QSet<TcpSocket *> setTcpSocket;
	static QReadWriteLock setLock;
};

#endif // TCPSOCKET_H
