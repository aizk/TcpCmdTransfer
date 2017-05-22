#ifndef TOPLATFORMHANDLER_H
#define TOPLATFORMHANDLER_H

#include "icmdhandler.h"
#include "httphandler.h"
#include <QtNetwork>

class PlatformToDevHandler : public ICmdHandler
{
public:
    PlatformToDevHandler();
	~PlatformToDevHandler();
    int handleCmd(QByteArray &aCmd);
	void setPlatformHostAddr(const QHostAddress &platformHostAddr);
    void setPlatformPort(const quint16 port);
    void setToDevSocket(QUdpSocket *pToDevSocket);
    void setPlatformRecvSocket(QUdpSocket *pPlatformRecvSocket);

protected:
    ICmdHandler *findDevHandler(quint32 devId);

private:
    QHostAddress *m_platformHostAddr;
    quint16 m_platformPort;
	QUdpSocket *m_platformRecvSocket;
    QUdpSocket *m_toDevSocket;
};

#endif // TOPLATFORMHANDLER_H
