/********************************************************************
	创建时间:	2016/8/9 17:00
	文件名: 	icmdtranslator.h
	作者:		余谨智
	目的:	    指令翻译器接口，用来处理指令内容。
*********************************************************************/
#ifndef ICMDTRANSLATOR_H
#define ICMDTRANSLATOR_H

#include <QtCore>


class ICmdTranslator
{
public:
	virtual ~ICmdTranslator();
	// 翻译指令
    virtual QByteArray & translate(QByteArray &aDest, const QByteArray &aSrc) = 0;

};

typedef ICmdTranslator *PCmdTranslator;

#endif // ICMDTRANSLATOR_H
