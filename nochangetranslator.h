/********************************************************************
	����ʱ��:	2016/8/9 17:01
	�ļ���: 	nochangetranslator.h
	����:		�����
	Ŀ��:	    �޸ı�ָ�����������ʲô�������ʲô��
*********************************************************************/
#ifndef NOTRANGETRANSLATOR_H
#define NOTRANGETRANSLATOR_H

#include "cmdtranslator.h"



class NoChangeTranslator : public CmdTranslator
{
public:
    NoChangeTranslator(const int iDirection);

protected:
    QByteArray & toDevCmd(QByteArray &aDevCmd, const QByteArray &aSrcCmd);
    QByteArray & fromDevCmd(QByteArray &aDestCmd, const QByteArray &aDevCmd);

};

#endif // NOTRANGETRANSLATOR_H
