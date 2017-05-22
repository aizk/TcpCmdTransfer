/********************************************************************
	创建时间:	2016/7/9 14:55
	文件名: 	alarmdev.cpp
	作者:		余谨智
	目的:	    报警设备类，其中含有报警设备的一些属性和方法，
				另外，还有管理所有报警设备的静态方法。
*********************************************************************/
#include "alarmdev.h"
#include "const.h"
#include "utils.h"
#include "cmdfactory.h"

QMap<quint32, AlarmDev *> AlarmDev::m_mapDevIdToPAlarmDev;
QReadWriteLock AlarmDev::mapLock;
CmdFactory *AlarmDev::m_pCmdFactory = NULL;

AlarmDev::AlarmDev()
    : devId(0),
      port(0),
      onlineStatus(true),
      m_noHeartTimes(-1),
	  m_noRtnTimes(-1),
	  m_bCmdExecing(0),
	  bufang(0),
	  m_bHeartTimingStart(false),
	  m_crc16(0)
{

}

AlarmDev::~AlarmDev()
{
}

//************************************
// 方法名称:    setHeartbeat
// 全名:        AlarmDev::setHeartbeat
// 可访问性:    public 
// 返回值:      void
// 限定符:     
// 说明：当接收到设备心跳时，将无心跳计算置0，
//       如果原来是离线状态时，则创建上线消息。
//************************************
void AlarmDev::setHeartbeat()
{
    m_noHeartTimes = 0; // 无心跳计时器重置为0
	if(!onlineStatus)
	{
		// 如果现在标记为离线，则设置为在线，
		// 并且制造一个设备上线命令
		onlineStatus = true;
		if(m_pCmdFactory)
		{
			m_pCmdFactory->createDevOnlineCmd(this);
		}
	}
	m_bHeartTimingStart = true; // 无心跳计时器重新开始计时
}

//************************************
// 方法名称:    isCmdTimeout
// 全名:        AlarmDev::isCmdTimeout
// 可访问性:    public 
// 返回值:      bool	命令是否超时
// 限定符:     
// 说明: 
//       
//************************************
bool AlarmDev::isCmdTimeout()
{
	if(Const::NO_CMD_RTN_TIME == m_noRtnTimes)
	{
		// 如果无指令返回计时次数已达到指定数值，则返回真，
		// 即指令下发超时。
		return true;
	}
	else
	{
		// 如果无指令返回计时次数未达到指定数值，则返回假，
		// 即指令下发未超时。
		return false;
	}
}

//************************************
// 方法名称:    startCmdTiming
// 全名:        AlarmDev::startCmdTiming
// 可访问性:    public 
// 返回值:      void
// 限定符:     
// 说明: 		开发指令下发计时
//       
//************************************
void AlarmDev::startCmdTiming()
{
	m_bCmdExecing = true;
	m_noRtnTimes = -1;
}

void AlarmDev::endCmdExecing()
{
	m_noRtnTimes = -1;
	m_bCmdExecing = false;
}

//************************************
// 方法名称:    incrementNoHeartTimes
// 全名:        AlarmDev::incrementNoHeartTimes
// 可访问性:    public 
// 返回值:      void
// 限定符:     
// 说明:		增加无心跳计数
//************************************
void AlarmDev::incrementNoHeartTimes()
{
    if(m_bHeartTimingStart && Const::NO_HEART_TIME == ++m_noHeartTimes)
    {
		Utils::remoteWinAlarm(devId);
        onlineStatus = false;
		m_pCmdFactory->createDevOnlineCmd(this);
        m_noHeartTimes = 0;
		m_bHeartTimingStart = false;
    }
}

//************************************
// 方法名称:    incrementTimeoutTimes
// 全名:        AlarmDev::incrementTimeoutTimes
// 可访问性:    public 
// 返回值:      void
// 限定符:     
// 说明:		递增该报警设备的指令超时计时器
//       
//************************************
void AlarmDev::incrementTimeoutTimes()
{
	if(m_bCmdExecing && !isCmdTimeout())
	{
		++m_noRtnTimes;
		
	}
}


//************************************
// 方法名称:    incrementAllTimer
// 全名:        AlarmDev::incrementAllTimer
// 可访问性:    public static 
// 返回值:      void
// 限定符:     
// 说明: 		递增所有设备的无心跳计时器及指令超时计时器
//       
//************************************
void AlarmDev::incrementAllTimer()
{
	QReadLocker locker(&mapLock);
	QList<AlarmDev *> pLstAlarmDev = m_mapDevIdToPAlarmDev.values();
	foreach (AlarmDev *pAlarmDev, pLstAlarmDev) {
		// 依次递增所有设备的无心跳计时器
		pAlarmDev->incrementNoHeartTimes();
		// 依次递增所有设备的指令超时计时器
		pAlarmDev->incrementTimeoutTimes();
	}
}

//************************************
// 方法名称:    isCmdExecing
// 全名:        AlarmDev::isCmdExecing
// 可访问性:    public 
// 返回值:      bool
// 限定符:     
// 说明:		是否有命令下发但仍未收到反馈
//       
//************************************
bool AlarmDev::isCmdExecing()
{
	return m_bCmdExecing;
}

//************************************
// 方法名称:    isCrcChanged
// 全名:        AlarmDev::isCrcChanged
// 可访问性:    public 
// 返回值:      bool
// 限定符:     
// 参数:        quint16 crc16	新心跳的CRC校验码
// 说明: 		与上一次心跳相比，CRC16校验码是否
//				改变。用来判断设备状态有无改变。
//       
//************************************
bool AlarmDev::isCrcChanged( quint16 crc16 )
{
	return m_crc16 != crc16;
}

//************************************
// 方法名称:    setCrc
// 全名:        AlarmDev::setCrc
// 可访问性:    public 
// 返回值:      void
// 限定符:     
// 参数:        quint16 crc16	新心跳数据的CRC校验码
// 说明: 		设置新心跳数据的CRC校验码
//       
//************************************
void AlarmDev::setCrc( quint16 crc16 )
{
	m_crc16 = crc16;
}


//************************************
// 方法名称:    setCmdFactory
// 全名:        AlarmDev::setCmdFactory
// 可访问性:    public static 
// 返回值:      void
// 限定符:     
// 参数:        ICmdFactory * pCmdFactory
// 说明: 		设置命令工厂
//       
//************************************
void AlarmDev::setCmdFactory( CmdFactory *pCmdFactory )
{
	m_pCmdFactory = pCmdFactory;
}

//************************************
// 方法名称:    insertAlarmDevToMap
// 全名:        AlarmDev::insertAlarmDevToMap
// 可访问性:    public static 
// 返回值:      int
// 限定符:     
// 参数:        AlarmDev * pAlarmDev 新的报警设备
// 说明: 		将新的报警设备加入设备ID到设备对象的map当中
//       
//************************************
int AlarmDev::insertAlarmDevToMap( AlarmDev *pAlarmDev )
{
	QWriteLocker locker(&mapLock);
	if(m_mapDevIdToPAlarmDev.contains(pAlarmDev->devId))
	{
		return Const::RTN_ERROR; // 如果已经存在同设备ID的设备对象，则返回插入失败
	}
	m_mapDevIdToPAlarmDev.insert(pAlarmDev->devId, pAlarmDev);
	return 0;
}


//************************************
// 方法名称:    delAlarmDevByDevId
// 全名:        AlarmDev::delAlarmDevByDevId
// 可访问性:    public static 
// 返回值:      int
// 限定符:     
// 参数:        quint32 devId	要删除的设备对象的设备ID
// 说明: 		根据设备ID删除指定设备对象
//       
//************************************
int AlarmDev::delAlarmDevByDevId( quint32 devId )
{
	QWriteLocker locker(&mapLock);
	if(m_mapDevIdToPAlarmDev.contains(devId))
	{
		AlarmDev *pTmpDev = m_mapDevIdToPAlarmDev.value(devId);
		delete pTmpDev;
		pTmpDev = NULL;
		m_mapDevIdToPAlarmDev.remove(devId);
		return 0;
	}
	else
	{
		return Const::RTN_ERROR;
	}
}

//************************************
// 方法名称:    getAlarmDevByDevId
// 全名:        AlarmDev::getAlarmDevByDevId
// 可访问性:    public static 
// 返回值:      AlarmDev *
// 限定符:     
// 参数:        quint32 devId	要获取的设备对象的设备ID
// 说明: 		根据设备ID来获取相对应的设备对象
//       
//************************************
AlarmDev * AlarmDev::getAlarmDevByDevId( quint32 devId )
{
	QReadLocker locker(&mapLock);
	if(m_mapDevIdToPAlarmDev.contains(devId))
	{
		return m_mapDevIdToPAlarmDev.value(devId);
	}
	else
	{
		return NULL;
	}
}


