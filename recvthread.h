/********************************************************************
	����ʱ��:	2016/8/9 17:13
	�ļ���: 	recvthread.h
	����:		�����
	Ŀ��:	    ƽ̨��������߳���
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
