/********************************************************************
	����ʱ��:	2016/7/9 14:55
	�ļ���: 	alarmdev.cpp
	����:		�����
	Ŀ��:	    �����豸�࣬���к��б����豸��һЩ���Ժͷ�����
				���⣬���й������б����豸�ľ�̬������
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
// ��������:    setHeartbeat
// ȫ��:        AlarmDev::setHeartbeat
// �ɷ�����:    public 
// ����ֵ:      void
// �޶���:     
// ˵���������յ��豸����ʱ����������������0��
//       ���ԭ��������״̬ʱ���򴴽�������Ϣ��
//************************************
void AlarmDev::setHeartbeat()
{
    m_noHeartTimes = 0; // ��������ʱ������Ϊ0
	if(!onlineStatus)
	{
		// ������ڱ��Ϊ���ߣ�������Ϊ���ߣ�
		// ��������һ���豸��������
		onlineStatus = true;
		if(m_pCmdFactory)
		{
			m_pCmdFactory->createDevOnlineCmd(this);
		}
	}
	m_bHeartTimingStart = true; // ��������ʱ�����¿�ʼ��ʱ
}

//************************************
// ��������:    isCmdTimeout
// ȫ��:        AlarmDev::isCmdTimeout
// �ɷ�����:    public 
// ����ֵ:      bool	�����Ƿ�ʱ
// �޶���:     
// ˵��: 
//       
//************************************
bool AlarmDev::isCmdTimeout()
{
	if(Const::NO_CMD_RTN_TIME == m_noRtnTimes)
	{
		// �����ָ��ؼ�ʱ�����Ѵﵽָ����ֵ���򷵻��棬
		// ��ָ���·���ʱ��
		return true;
	}
	else
	{
		// �����ָ��ؼ�ʱ����δ�ﵽָ����ֵ���򷵻ؼ٣�
		// ��ָ���·�δ��ʱ��
		return false;
	}
}

//************************************
// ��������:    startCmdTiming
// ȫ��:        AlarmDev::startCmdTiming
// �ɷ�����:    public 
// ����ֵ:      void
// �޶���:     
// ˵��: 		����ָ���·���ʱ
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
// ��������:    incrementNoHeartTimes
// ȫ��:        AlarmDev::incrementNoHeartTimes
// �ɷ�����:    public 
// ����ֵ:      void
// �޶���:     
// ˵��:		��������������
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
// ��������:    incrementTimeoutTimes
// ȫ��:        AlarmDev::incrementTimeoutTimes
// �ɷ�����:    public 
// ����ֵ:      void
// �޶���:     
// ˵��:		�����ñ����豸��ָ�ʱ��ʱ��
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
// ��������:    incrementAllTimer
// ȫ��:        AlarmDev::incrementAllTimer
// �ɷ�����:    public static 
// ����ֵ:      void
// �޶���:     
// ˵��: 		���������豸����������ʱ����ָ�ʱ��ʱ��
//       
//************************************
void AlarmDev::incrementAllTimer()
{
	QReadLocker locker(&mapLock);
	QList<AlarmDev *> pLstAlarmDev = m_mapDevIdToPAlarmDev.values();
	foreach (AlarmDev *pAlarmDev, pLstAlarmDev) {
		// ���ε��������豸����������ʱ��
		pAlarmDev->incrementNoHeartTimes();
		// ���ε��������豸��ָ�ʱ��ʱ��
		pAlarmDev->incrementTimeoutTimes();
	}
}

//************************************
// ��������:    isCmdExecing
// ȫ��:        AlarmDev::isCmdExecing
// �ɷ�����:    public 
// ����ֵ:      bool
// �޶���:     
// ˵��:		�Ƿ��������·�����δ�յ�����
//       
//************************************
bool AlarmDev::isCmdExecing()
{
	return m_bCmdExecing;
}

//************************************
// ��������:    isCrcChanged
// ȫ��:        AlarmDev::isCrcChanged
// �ɷ�����:    public 
// ����ֵ:      bool
// �޶���:     
// ����:        quint16 crc16	��������CRCУ����
// ˵��: 		����һ��������ȣ�CRC16У�����Ƿ�
//				�ı䡣�����ж��豸״̬���޸ı䡣
//       
//************************************
bool AlarmDev::isCrcChanged( quint16 crc16 )
{
	return m_crc16 != crc16;
}

//************************************
// ��������:    setCrc
// ȫ��:        AlarmDev::setCrc
// �ɷ�����:    public 
// ����ֵ:      void
// �޶���:     
// ����:        quint16 crc16	���������ݵ�CRCУ����
// ˵��: 		�������������ݵ�CRCУ����
//       
//************************************
void AlarmDev::setCrc( quint16 crc16 )
{
	m_crc16 = crc16;
}


//************************************
// ��������:    setCmdFactory
// ȫ��:        AlarmDev::setCmdFactory
// �ɷ�����:    public static 
// ����ֵ:      void
// �޶���:     
// ����:        ICmdFactory * pCmdFactory
// ˵��: 		���������
//       
//************************************
void AlarmDev::setCmdFactory( CmdFactory *pCmdFactory )
{
	m_pCmdFactory = pCmdFactory;
}

//************************************
// ��������:    insertAlarmDevToMap
// ȫ��:        AlarmDev::insertAlarmDevToMap
// �ɷ�����:    public static 
// ����ֵ:      int
// �޶���:     
// ����:        AlarmDev * pAlarmDev �µı����豸
// ˵��: 		���µı����豸�����豸ID���豸�����map����
//       
//************************************
int AlarmDev::insertAlarmDevToMap( AlarmDev *pAlarmDev )
{
	QWriteLocker locker(&mapLock);
	if(m_mapDevIdToPAlarmDev.contains(pAlarmDev->devId))
	{
		return Const::RTN_ERROR; // ����Ѿ�����ͬ�豸ID���豸�����򷵻ز���ʧ��
	}
	m_mapDevIdToPAlarmDev.insert(pAlarmDev->devId, pAlarmDev);
	return 0;
}


//************************************
// ��������:    delAlarmDevByDevId
// ȫ��:        AlarmDev::delAlarmDevByDevId
// �ɷ�����:    public static 
// ����ֵ:      int
// �޶���:     
// ����:        quint32 devId	Ҫɾ�����豸������豸ID
// ˵��: 		�����豸IDɾ��ָ���豸����
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
// ��������:    getAlarmDevByDevId
// ȫ��:        AlarmDev::getAlarmDevByDevId
// �ɷ�����:    public static 
// ����ֵ:      AlarmDev *
// �޶���:     
// ����:        quint32 devId	Ҫ��ȡ���豸������豸ID
// ˵��: 		�����豸ID����ȡ���Ӧ���豸����
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


