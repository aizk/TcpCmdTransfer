#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QtNetwork>

class TcpCmdRecv;
class TcpSocket;
class TcpThread;
class TcpCmdFactory;

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    TcpServer();
	~TcpServer();
	void setTcpCmdRecv(TcpCmdRecv *pTcpCmdRecv);
	void setTcpCmdFactory(TcpCmdFactory *pTcpCmdFactory);
    //static TcpSocket *getTcpSocketByDevId(quint32 devId);
	int addDevIdTcpSock(quint32 devId, TcpSocket *pTcpSocket);
    void createTcpThread();
	void setListenPort(quint16 listenPort);
	bool startListen();
	void setIsDev(bool isDev);
	bool isDev();

protected:
	//TcpSocket *createTcpSocket();
    void incomingConnection(qintptr socketDescriptor);

protected:
    QList<TcpThread *> m_lstTcpThread;
    QList<TcpSocket *> m_lstPTcpSocket;
    TcpCmdRecv *m_pTcpCmdRecv;
	TcpCmdFactory *m_pTcpCmdFactory;
    quint16 m_listenPort;
	bool m_isDev;
    
};

#endif // TCPSERVER_H
