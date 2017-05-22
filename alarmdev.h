/********************************************************************
	创建时间:	2016/8/2 9:18
	文件名: 	alarmdev.h
	作者:		余谨智
	目的:	    报警设备类，其中含有报警设备的一些属性和方法，
				另外，还有管理所有报警设备的静态方法。
*********************************************************************/
#ifndef ALARMDEV_H
#define ALARMDEV_H

#include <QtCore>
#include <QtNetwork>
#include "tcpsocket.h"

class CmdFactory;

class AlarmDev
{
public:
    AlarmDev();
	~AlarmDev();
    quint32 devId; // 设备ID
    QHostAddress hostAddr; // 报警设备主机地址
    quint16 port; // 报警设备端口
	quint8 bufang; // 报警设备布撤防状态标识
    bool onlineStatus; // 报警设备在线离线状态标识

	

    void incrementNoHeartTimes(); // 递增该报警设备的无心跳计时器

	void incrementTimeoutTimes(); // 递增该报警设备的指令超时计时器

	bool isCmdExecing(); // 是否有指令发送到报警设备而还未收到执行反馈

	void setHeartbeat(); // 当接收到设备心跳时，将无心跳计算置0，
						 // 如果原来是离线状态时，则创建上线消息。
	void startCmdTiming(); // 开始命令下发执行计时

	void endCmdExecing();  // 结束命令下发执行计时

	bool isCrcChanged(quint16 crc16); // 设备心跳的CRC16校验码是否改变
	void setCrc(quint16 crc16); // 设置此次设备心跳CRC16校验码
inline	bool isCmdTimeout(); // 指令是否超时

public:
	static void setCmdFactory(CmdFactory *pCmdFactory); // 设置命令工厂

	static int insertAlarmDevToMap(AlarmDev *pAlarmDev); // 添加新的报警设备

	static void incrementAllTimer(); // 递增所有的计时器，如指令超时计时器，无心跳计时器

	static AlarmDev *getAlarmDevByDevId(quint32 devId); // 通过设备ID来获取报警设备对象

	static int delAlarmDevByDevId(quint32 devId); // 通过设备ID来删除对应的报警设备对象

private:
	quint16 m_crc16; // 设备心跳的CRC16校验码
    volatile int m_noHeartTimes; // 无心跳计时器
	volatile int m_noRtnTimes;   // 无指令反馈计时器
	volatile bool m_bCmdExecing; // 指令是否已下发
	volatile bool m_bHeartTimingStart; // 心跳计时开始
	static CmdFactory *m_pCmdFactory; // 命令工厂
	static QMap<quint32, AlarmDev *> m_mapDevIdToPAlarmDev; // 设备ID到报警设备的map
	static QReadWriteLock mapLock; // 服务于m_mapDevIdToPAlarmDev的读写锁
};

#endif // ALARMDEV_H
