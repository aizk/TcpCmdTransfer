/********************************************************************
	创建时间:	2016/8/9 17:10
	文件名: 	platformcmdrecv.h
	作者:		余谨智
	目的:	    平台命令接收器
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
