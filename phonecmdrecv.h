/********************************************************************
	����ʱ��:	2016/8/9 17:03
	�ļ���: 	phonecmdrecv.h
	����:		�����
	Ŀ��:	    �ֻ�APPָ�������
*********************************************************************/
#ifndef PHONEDEVRECV_H
#define PHONEDEVRECV_H

#include "udpcmdrecv.h"

class PhoneCmdRecv : public UDPCmdRecv
{
public:
	PhoneCmdRecv(CmdFactory *pCmdFactory, QUdpSocket *pUdpSocket);
    void createCmd(); // �����������

};

#endif // PHONEDEVRECV_H
