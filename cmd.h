/********************************************************************
	����ʱ��:	2016/8/5 16:06
	�ļ���: 	cmd.h
	����:		�����
	Ŀ��:	    ���������ͷ�ļ�
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
	// �����µķ������ʹ�����
    void addTranslatorAndHandlers(
            PCmdTranslator pCmdTranslator,
            const ListPCmdHandler &lstPCmdHandler
            );
    int handle(); // �����������

private:
    QList<PCmdTranslator> m_lstPCmdTranslator; // ������list
    QMap<PCmdTranslator, ListPCmdHandler> m_mapPCmdTranslatorToLstPHandler; // ����������Ӧ�Ĵ�����
    QByteArray m_recvCmd; // ���յ��������ֽ���
};

#endif // CMD_H
