/********************************************************************
	创建时间:	2016/8/9 17:01
	文件名: 	nochangetranslator.h
	作者:		余谨智
	目的:	    无改变指令翻译器。输入什么，就输出什么。
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
