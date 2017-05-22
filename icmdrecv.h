/********************************************************************
	����ʱ��:	2016/8/9 16:21
	�ļ���: 	icmdrecv.h
	����:		�����
	Ŀ��:	    ָ��������ӿ�
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
