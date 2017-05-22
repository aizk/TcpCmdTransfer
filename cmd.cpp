/********************************************************************
	创建时间:	2016/8/5 16:22
	文件名: 	cmd.cpp
	作者:		余谨智
	目的:	    命令对象类的实现文件
*********************************************************************/
#include "cmd.h"
#include "const.h"
#include "cmdtranslator.h"
#include "utils.h"

Cmd::Cmd(const QByteArray &recvCmd)
{
    m_recvCmd = recvCmd;
}

Cmd::~Cmd()
{

}

//************************************
// 方法名称:    addTranslatorAndHandlers
// 全名:        Cmd::addTranslatorAndHandlers
// 可访问性:    public 
// 返回值:      void
// 限定符:     
// 参数:        PCmdTranslator pCmdTranslator	指向翻译器对象的指针
// 参数:        const ListPCmdHandler & lstPCmdHandler	指向命令处理器指针的list
// 说明: 		向命令对象添加翻译器及相应的处理器
//       
//************************************
void Cmd::addTranslatorAndHandlers(PCmdTranslator pCmdTranslator, const ListPCmdHandler &lstPCmdHandler)
{
	m_lstPCmdTranslator.append(pCmdTranslator);
    m_mapPCmdTranslatorToLstPHandler.insert(pCmdTranslator, lstPCmdHandler);
}

//************************************
// 方法名称:    handle
// 全名:        Cmd::handle
// 可访问性:    public 
// 返回值:      int	返回0，则为处理成功，否则为处理失败。
// 限定符:     
// 说明: 		整个指令转发程序的核心部分。
//				运用翻译器和处理器对命令对象进行处理。
//       
//************************************
int Cmd::handle()
{
    if(m_mapPCmdTranslatorToLstPHandler.count() == 0)
        return Const::RTN_ERROR; // 如果该命令对象无任何翻译器，则直接返回错误
    QByteArray destCmd;
    ListPCmdHandler lstCmdHandlers;
    int ret = 0;
	foreach(const PCmdTranslator &cmdTranslator, m_lstPCmdTranslator)
	{
		// 对于每一个翻译器，先获取翻译器下的处理器
		lstCmdHandlers = m_mapPCmdTranslatorToLstPHandler[cmdTranslator];
		// 运用翻译器翻译接收到的指令
		cmdTranslator->translate(destCmd, m_recvCmd);
		if(destCmd.size() == 0)
			// 如果翻译后的指令长度为0，则无需进行任何处理，
			// 进入下一个翻译器的操作
			continue; 
		
		foreach(const PCmdHandler &pCmdHandler, lstCmdHandlers)
		{
			// 对于翻译器翻译后的指令，运用其对应的处理器对翻译后的指令进行处理
			ret = pCmdHandler->handleCmd(destCmd);
			// 如果命令处理出现错误，则返回执行错误
			if(Const::RTN_ERROR == ret)
				return Const::RTN_ERROR;
		}
	}
    return 0;
}
