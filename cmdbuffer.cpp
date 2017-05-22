/********************************************************************
	����ʱ��:	2016/8/5 17:13
	�ļ���: 	cmdbuffer.cpp
	����:		�����
	Ŀ��:	    ָ����࣬��֤һ��ָ��������ԺͿɿ��ԡ�
				���ճ�������ݴ����������⡣
*********************************************************************/
#include "cmdbuffer.h"
#include "const.h"
#include "crc16.h"

using namespace Const;

CmdBuffer::CmdBuffer()
	: m_bHasHead(false)
{
    m_buffer.reserve(CMD_BUFFER_SIZE);
}

//************************************
// ��������:    putData
// ȫ��:        CmdBuffer::putData
// �ɷ�����:    public 
// ����ֵ:      void
// �޶���:     
// ����:        const QByteArray & data	�µ�����
// ˵��: 		���½��յ������ݷ���ָ�����
//       
//************************************
void CmdBuffer::putData(const QByteArray &data)
{
    m_buffer.append(data);
}

//************************************
// ��������:    nextCompleteCmd
// ȫ��:        CmdBuffer::nextCompleteCmd
// �ɷ�����:    public 
// ����ֵ:      QT_NAMESPACE::QByteArray
// �޶���:     
// ˵��: 		��ȡ��һ����������Ч��ָ��
//       
//************************************
QByteArray CmdBuffer::nextCompleteCmd()
{
    QByteArray rtnArray; // ���ֽ�����

	int i = 0;
	do 
	{
		// Ѱ�ҵ�֡��ʼ��ʶ����4��0xFF
		for(i = 0; i < m_buffer.size(); ++i)
		{	// �ҵ���һ��0xFF�ֽ�
			if(FRAME_START_FLAG_BYTE == m_buffer[i])
				break;
		}
		m_buffer.remove(0, i); // ����һ��0xFF�ֽ�ǰ�������ֽ��Ƴ�
		// ���������ʣ����ֽ���δ�ﵽ4���ֽڣ��򷵻ؿգ�
		// �ȴ���һ���ֽڵĵ���
		if(m_buffer.size() < CMD_FRAME_START_FLAG_SIZE)
			return rtnArray;
		// �鿴��������ǰ4���ֽ��Ƿ���0xFF��
		// ���ǰ4���ֽ�����һ���ֽڲ���0xFF,
		// ��ɾ���ò���0xFF���ֽڼ�֮ǰ��0xFF�ֽ�,
		// ���ǰ4���ֽڶ���0xFF�����˳�����4������0xFF��ѭ��
		for(i = 0; i < CMD_FRAME_START_FLAG_SIZE; ++i)
		{
			if(FRAME_START_FLAG_BYTE != m_buffer[i])
				break;
		}
		if(CMD_FRAME_START_FLAG_SIZE != i)
			m_buffer.remove(0, i);
		else
			break;
	} while (m_buffer.size() > 0);
	// ����������е����ݻ�����֡ͷ���ȣ�
	// �򷵻ؿգ��ȴ���һ���ֽڵ�����
	if(m_buffer.size() < CMD_HEAD_SIZE)
		return rtnArray;
	// �������˵�����������Ѱ���ָ��֡ͷ
	m_bHasHead = true; // ���Ƿ����֡ͷ��־��Ϊ��
	int cmdSize = 0; // ����ָ��Ӧ�г���
	quint8 devType = 0;
	quint32 devId = 0;
	quint16 cmdId = 0;
	quint16 mid = 0;
	quint16 dataLen = 0;
	quint16 crc16 = 0;

	// �ӻ������л�ȡ�豸���ͣ��豸ID������ID, MID�����ݳ���
	QDataStream bs(m_buffer);
	bs.skipRawData(CMD_FRAME_START_FLAG_SIZE);
	bs >> devType >> devId >> cmdId >> mid >> dataLen;
	if(dataLen > MAX_CMD_LEN)
	{
		// ������ݳ��ȳ�������ֵ��˵�����ڴ���
		// ����ջ����������ؿ�
		m_buffer.clear();
		return rtnArray;
	}
	// ���������ָ��Ӧ�еĳ���
    cmdSize = MIN_CMD_SIZE + dataLen;
	// ������������ַ���С�ڸ���ָ��Ӧ�еĳ��ȣ�
	// �򷵻ؿգ��ȴ���һ������
	if(m_buffer.size() < cmdSize)
		return rtnArray;
	// ��������Ӧ���Ѱ���һ������������֡��
	bs.skipRawData(dataLen);
	bs >> crc16; // ��ȡָ��֡�е�CRC16У����
	// �������ָ���CRC16����������ݵ�CRC16У���룬��
	// ��ָ��֡�л�ȡ��У������бȽ�
    QByteArray tempFrame = m_buffer.left(cmdSize - 2);
	quint16 crc16Checksum = Utils::modbus_crc((quint8 *)tempFrame.data(), tempFrame.size());
	if(crc16 != crc16Checksum)
	{
		// ���У���벻ƥ�䣬��ӻ�������ɾ����֡�����ؿ�
		m_buffer.remove(0, cmdSize);
		return rtnArray;
	}
	// �Ƴ�ָ��֡ǰ��4��0xFF��ʼ��ʶ
	m_buffer.remove(0, CMD_FRAME_START_FLAG_SIZE);
	// �ӻ���������ȡ��һ����������Ч��ָ��֡
	rtnArray = m_buffer.left(cmdSize - CMD_FRAME_START_FLAG_SIZE);
	// �Ƴ���ָ��֡
	m_buffer.remove(0, cmdSize - CMD_FRAME_START_FLAG_SIZE);
	// ���Ƿ����ָ��֡ͷ��ʶ��Ϊfalse
	m_bHasHead = false;
	// ���ظ���������Ч��ָ��֡
    return rtnArray;
}


