#ifndef TCPHANDLER_H
#define TCPHANDLER_H

#include "icmdhandler.h"
#include "tcpsocket.h"

class TcpHandler : public ICmdHandler
{
	Q_OBJECT
public:
    TcpHandler(TcpSocket *pTcpSocket);
    ~TcpHandler();
    int handleCmd(QByteArray &aCmd);
	TcpSocket *getTcpSocket();

    void setTcpSocket(TcpSocket *pTcpSocket);
	void setValid(bool isValid);
	bool isValid();

signals:
	void toSendData(const QByteArray &aCmd);

private:
    TcpSocket *m_pTcpSocket;
	volatile bool m_isValid;

};

#endif // TCPHANDLER_H
