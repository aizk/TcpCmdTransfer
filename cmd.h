/********************************************************************
	创建时间:	2016/8/5 16:06
	文件名: 	cmd.h
	作者:		余谨智
	目的:	    命令对象类头文件
*********************************************************************/
#ifndef CMD_H
#define CMD_H

#include <QtCore>
#include "icmdtranslator.h"
#include "icmdhandler.h"

class Cmd
{
public:
    Cmd(const QByteArray &recvCmd);
	~Cmd();
	// 增加新的翻译器和处理器
    void addTranslatorAndHandlers(
            PCmdTranslator pCmdTranslator,
            const ListPCmdHandler &lstPCmdHandler
            );
    int handle(); // 处理命令对象

private:
    QList<PCmdTranslator> m_lstPCmdTranslator; // 翻译器list
    QMap<PCmdTranslator, ListPCmdHandler> m_mapPCmdTranslatorToLstPHandler; // 翻译器及对应的处理器
    QByteArray m_recvCmd; // 接收到的命令字节流
};

#endif // CMD_H
