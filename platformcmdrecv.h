/********************************************************************
	����ʱ��:	2016/8/9 17:10
	�ļ���: 	platformcmdrecv.h
	����:		�����
	Ŀ��:	    ƽ̨���������
*********************************************************************/
#ifndef PLATFORMCMDRECV_H
#define PLATFORMCMDRECV_H

#include "udpcmdrecv.h"
#include "cmdfactory.h"

class PlatformCmdRecv : public UDPCmdRecv
{
public:
    PlatformCmdRecv(CmdFactory *pCmdFactory, QUdpSocket *pUdpSocket);
    void createCmd();

};

#endif // PLATFORMCMDRECV_H
