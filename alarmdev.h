/********************************************************************
	����ʱ��:	2016/8/2 9:18
	�ļ���: 	alarmdev.h
	����:		�����
	Ŀ��:	    �����豸�࣬���к��б����豸��һЩ���Ժͷ�����
				���⣬���й������б����豸�ľ�̬������
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
    quint32 devId; // �豸ID
    QHostAddress hostAddr; // �����豸������ַ
    quint16 port; // �����豸�˿�
	quint8 bufang; // �����豸������״̬��ʶ
    bool onlineStatus; // �����豸��������״̬��ʶ

	

    void incrementNoHeartTimes(); // �����ñ����豸����������ʱ��

	void incrementTimeoutTimes(); // �����ñ����豸��ָ�ʱ��ʱ��

	bool isCmdExecing(); // �Ƿ���ָ��͵������豸����δ�յ�ִ�з���

	void setHeartbeat(); // �����յ��豸����ʱ����������������0��
						 // ���ԭ��������״̬ʱ���򴴽�������Ϣ��
	void startCmdTiming(); // ��ʼ�����·�ִ�м�ʱ

	void endCmdExecing();  // ���������·�ִ�м�ʱ

	bool isCrcChanged(quint16 crc16); // �豸������CRC16У�����Ƿ�ı�
	void setCrc(quint16 crc16); // ���ô˴��豸����CRC16У����
inline	bool isCmdTimeout(); // ָ���Ƿ�ʱ

public:
	static void setCmdFactory(CmdFactory *pCmdFactory); // ���������

	static int insertAlarmDevToMap(AlarmDev *pAlarmDev); // ����µı����豸

	static void incrementAllTimer(); // �������еļ�ʱ������ָ�ʱ��ʱ������������ʱ��

	static AlarmDev *getAlarmDevByDevId(quint32 devId); // ͨ���豸ID����ȡ�����豸����

	static int delAlarmDevByDevId(quint32 devId); // ͨ���豸ID��ɾ����Ӧ�ı����豸����

private:
	quint16 m_crc16; // �豸������CRC16У����
    volatile int m_noHeartTimes; // ��������ʱ��
	volatile int m_noRtnTimes;   // ��ָ�����ʱ��
	volatile bool m_bCmdExecing; // ָ���Ƿ����·�
	volatile bool m_bHeartTimingStart; // ������ʱ��ʼ
	static CmdFactory *m_pCmdFactory; // �����
	static QMap<quint32, AlarmDev *> m_mapDevIdToPAlarmDev; // �豸ID�������豸��map
	static QReadWriteLock mapLock; // ������m_mapDevIdToPAlarmDev�Ķ�д��
};

#endif // ALARMDEV_H
