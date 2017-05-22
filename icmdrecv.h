/********************************************************************
	创建时间:	2016/8/9 16:21
	文件名: 	icmdrecv.h
	作者:		余谨智
	目的:	    指令接收器接口
*********************************************************************/
#ifndef ICMDRECV_H
#define ICMDRECV_H

#include <QtCore>

class ICmdRecv : public QObject
{
public:
	virtual ~ICmdRecv();
    virtual bool hasCmdData() = 0;
    virtual void createCmd() = 0;
};

typedef ICmdRecv *PICmdRecv;
#endif // ICMDRECV_H
