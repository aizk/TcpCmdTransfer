/********************************************************************
	创建时间:	2016/8/5 17:04
	文件名: 	cmdbuffer.h
	作者:		余谨智
	目的:	    指令缓冲区类，保证一条指令的完整性和可靠性。
				解决粘包，数据传输出错等问题。
*********************************************************************/
#ifndef CMDBUFFER_H
#define CMDBUFFER_H

#include <QtCore>

class CmdBuffer
{
public:
    CmdBuffer();

    void putData(const QByteArray &data); // 将新的数据放入缓存区中
    QByteArray nextCompleteCmd(); // 获取下一帧数据

private:
    QByteArray m_buffer; // 指令缓冲区

	// 缓冲区是否已包含帧头（4个0xFF + 设备类型 + 设备ID + 指令ID + MID + 数据长度）
	// 具体详见报警板通信协议
	bool m_bHasHead; 
};

#endif // CMDBUFFER_H
