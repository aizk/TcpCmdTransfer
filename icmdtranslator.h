/********************************************************************
	����ʱ��:	2016/8/9 17:00
	�ļ���: 	icmdtranslator.h
	����:		�����
	Ŀ��:	    ָ������ӿڣ���������ָ�����ݡ�
*********************************************************************/
#ifndef ICMDTRANSLATOR_H
#define ICMDTRANSLATOR_H

#include <QtCore>


class ICmdTranslator
{
public:
	virtual ~ICmdTranslator();
	// ����ָ��
    virtual QByteArray & translate(QByteArray &aDest, const QByteArray &aSrc) = 0;

};

typedef ICmdTranslator *PCmdTranslator;

#endif // ICMDTRANSLATOR_H
