/********************************************************************
	创建时间:	2016/8/5 17:13
	文件名: 	cmdbuffer.cpp
	作者:		余谨智
	目的:	    指令缓存类，保证一条指令的完整性和可靠性。
				解决粘包，数据传输出错等问题。
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
// 方法名称:    putData
// 全名:        CmdBuffer::putData
// 可访问性:    public 
// 返回值:      void
// 限定符:     
// 参数:        const QByteArray & data	新的数据
// 说明: 		将新接收到的数据放入指令缓存区
//       
//************************************
void CmdBuffer::putData(const QByteArray &data)
{
    m_buffer.append(data);
}

//************************************
// 方法名称:    nextCompleteCmd
// 全名:        CmdBuffer::nextCompleteCmd
// 可访问性:    public 
// 返回值:      QT_NAMESPACE::QByteArray
// 限定符:     
// 说明: 		获取下一个完整而有效的指令
//       
//************************************
QByteArray CmdBuffer::nextCompleteCmd()
{
    QByteArray rtnArray; // 空字节数组

	int i = 0;
	do 
	{
		// 寻找到帧起始标识，即4个0xFF
		for(i = 0; i < m_buffer.size(); ++i)
		{	// 找到第一个0xFF字节
			if(FRAME_START_FLAG_BYTE == m_buffer[i])
				break;
		}
		m_buffer.remove(0, i); // 将第一个0xFF字节前的所有字节移除
		// 如果缓冲区剩余的字节数未达到4个字节，则返回空，
		// 等待下一批字节的到来
		if(m_buffer.size() < CMD_FRAME_START_FLAG_SIZE)
			return rtnArray;
		// 查看缓冲区中前4个字节是否都是0xFF，
		// 如果前4个字节中有一个字节不是0xFF,
		// 则删除该不是0xFF的字节及之前的0xFF字节,
		// 如果前4个字节都是0xFF，则退出查找4个连续0xFF的循环
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
	// 如果缓存区中的数据还不到帧头长度，
	// 则返回空，等待下一批字节到来。
	if(m_buffer.size() < CMD_HEAD_SIZE)
		return rtnArray;
	// 到达这里，说明缓冲区中已包含指令帧头
	m_bHasHead = true; // 将是否包含帧头标志置为真
	int cmdSize = 0; // 该条指令应有长度
	quint8 devType = 0;
	quint32 devId = 0;
	quint16 cmdId = 0;
	quint16 mid = 0;
	quint16 dataLen = 0;
	quint16 crc16 = 0;

	// 从缓冲区中获取设备类型，设备ID，命令ID, MID，数据长度
	QDataStream bs(m_buffer);
	bs.skipRawData(CMD_FRAME_START_FLAG_SIZE);
	bs >> devType >> devId >> cmdId >> mid >> dataLen;
	if(dataLen > MAX_CMD_LEN)
	{
		// 如果数据长度超过正常值，说明存在错误，
		// 则清空缓冲区，返回空
		m_buffer.clear();
		return rtnArray;
	}
	// 计算出该条指令应有的长度
    cmdSize = MIN_CMD_SIZE + dataLen;
	// 如果缓冲区的字符数小于该条指令应有的长度，
	// 则返回空，等待下一批数据
	if(m_buffer.size() < cmdSize)
		return rtnArray;
	// 缓冲区中应该已包含一个完整的数据帧了
	bs.skipRawData(dataLen);
	bs >> crc16; // 获取指令帧中的CRC16校验码
	// 计算该条指令除CRC16段以外的数据的CRC16校验码，和
	// 从指令帧中获取的校验码进行比较
    QByteArray tempFrame = m_buffer.left(cmdSize - 2);
	quint16 crc16Checksum = Utils::modbus_crc((quint8 *)tempFrame.data(), tempFrame.size());
	if(crc16 != crc16Checksum)
	{
		// 如果校验码不匹配，则从缓冲区中删除该帧，返回空
		m_buffer.remove(0, cmdSize);
		return rtnArray;
	}
	// 移除指令帧前的4个0xFF起始标识
	m_buffer.remove(0, CMD_FRAME_START_FLAG_SIZE);
	// 从缓存区中提取出一条完整而有效的指令帧
	rtnArray = m_buffer.left(cmdSize - CMD_FRAME_START_FLAG_SIZE);
	// 移除该指令帧
	m_buffer.remove(0, cmdSize - CMD_FRAME_START_FLAG_SIZE);
	// 将是否包含指令帧头标识置为false
	m_bHasHead = false;
	// 返回该完整而有效的指令帧
    return rtnArray;
}


