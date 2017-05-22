/********************************************************************
	����ʱ��:	2016/8/5 16:22
	�ļ���: 	cmd.cpp
	����:		�����
	Ŀ��:	    ����������ʵ���ļ�
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
// ��������:    addTranslatorAndHandlers
// ȫ��:        Cmd::addTranslatorAndHandlers
// �ɷ�����:    public 
// ����ֵ:      void
// �޶���:     
// ����:        PCmdTranslator pCmdTranslator	ָ�����������ָ��
// ����:        const ListPCmdHandler & lstPCmdHandler	ָ���������ָ���list
// ˵��: 		�����������ӷ���������Ӧ�Ĵ�����
//       
//************************************
void Cmd::addTranslatorAndHandlers(PCmdTranslator pCmdTranslator, const ListPCmdHandler &lstPCmdHandler)
{
	m_lstPCmdTranslator.append(pCmdTranslator);
    m_mapPCmdTranslatorToLstPHandler.insert(pCmdTranslator, lstPCmdHandler);
}

//************************************
// ��������:    handle
// ȫ��:        Cmd::handle
// �ɷ�����:    public 
// ����ֵ:      int	����0����Ϊ����ɹ�������Ϊ����ʧ�ܡ�
// �޶���:     
// ˵��: 		����ָ��ת������ĺ��Ĳ��֡�
//				���÷������ʹ����������������д���
//       
//************************************
int Cmd::handle()
{
    if(m_mapPCmdTranslatorToLstPHandler.count() == 0)
        return Const::RTN_ERROR; // ���������������κη���������ֱ�ӷ��ش���
    QByteArray destCmd;
    ListPCmdHandler lstCmdHandlers;
    int ret = 0;
	foreach(const PCmdTranslator &cmdTranslator, m_lstPCmdTranslator)
	{
		// ����ÿһ�����������Ȼ�ȡ�������µĴ�����
		lstCmdHandlers = m_mapPCmdTranslatorToLstPHandler[cmdTranslator];
		// ���÷�����������յ���ָ��
		cmdTranslator->translate(destCmd, m_recvCmd);
		if(destCmd.size() == 0)
			// ���������ָ���Ϊ0������������κδ���
			// ������һ���������Ĳ���
			continue; 
		
		foreach(const PCmdHandler &pCmdHandler, lstCmdHandlers)
		{
			// ���ڷ�����������ָ��������Ӧ�Ĵ������Է�����ָ����д���
			ret = pCmdHandler->handleCmd(destCmd);
			// ����������ִ����򷵻�ִ�д���
			if(Const::RTN_ERROR == ret)
				return Const::RTN_ERROR;
		}
	}
    return 0;
}
