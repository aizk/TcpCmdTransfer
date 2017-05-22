/********************************************************************
	创建时间:	2016/8/9 17:03
	文件名: 	phonecmdrecv.h
	作者:		余谨智
	目的:	    手机APP指令接收器
*********************************************************************/
#ifndef PHONEDEVRECV_H
#define PHONEDEVRECV_H

#include "udpcmdrecv.h"

class PhoneCmdRecv : public UDPCmdRecv
{
public:
	PhoneCmdRecv(CmdFactory *pCmdFactory, QUdpSocket *pUdpSocket);
    void createCmd(); // 创建命令对象

};

#endif // PHONEDEVRECV_H
