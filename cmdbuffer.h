/********************************************************************
	����ʱ��:	2016/8/5 17:04
	�ļ���: 	cmdbuffer.h
	����:		�����
	Ŀ��:	    ָ������࣬��֤һ��ָ��������ԺͿɿ��ԡ�
				���ճ�������ݴ����������⡣
*********************************************************************/
#ifndef CMDBUFFER_H
#define CMDBUFFER_H

#include <QtCore>

class CmdBuffer
{
public:
    CmdBuffer();

    void putData(const QByteArray &data); // ���µ����ݷ��뻺������
    QByteArray nextCompleteCmd(); // ��ȡ��һ֡����

private:
    QByteArray m_buffer; // ָ�����

	// �������Ƿ��Ѱ���֡ͷ��4��0xFF + �豸���� + �豸ID + ָ��ID + MID + ���ݳ��ȣ�
	// �������������ͨ��Э��
	bool m_bHasHead; 
};

#endif // CMDBUFFER_H
