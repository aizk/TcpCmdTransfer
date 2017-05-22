/********************************************************************
	创建时间:	2016/8/9 17:13
	文件名: 	recvthread.h
	作者:		余谨智
	目的:	    平台命令接收线程类
*********************************************************************/
#ifndef RECVTHREAD_H
#define RECVTHREAD_H

#include <QtCore>
#include "icmdrecv.h"
#include "cmdfactory.h"

class RecvThread : public QThread
{
public:
    RecvThread();
	~RecvThread();
    void addCmdRecv(PICmdRecv pCmdRecv);
	void setCmdFactory(CmdFactory *pCmdFactory);
	void start(Priority = InheritPriority);

private:
    volatile bool m_bStartToEnd;
    QList<PICmdRecv> m_lstPICmdRecv;
	CmdFactory *m_pCmdFactory;
	QUdpSocket *m_pPlatformRecvSocket;
};

#endif // RECVTHREAD_H
