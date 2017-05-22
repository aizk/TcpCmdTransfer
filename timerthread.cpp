#include "timerthread.h"
#include "alarmdev.h"
#include "const.h"
#include "tcpsocket.h"


TimerThread::TimerThread()
	: m_bStartToEnd(false),
	  m_pTimer(NULL),
	  m_iTcpTimer(0)
{
}

void TimerThread::run()
{
	m_pTimer = new QTimer();
	m_pTimer->moveToThread(this);
	m_pTimer->setSingleShot(false);
	m_pTimer->setInterval(Const::TIMER_INTERVAL);
	QObject::connect(m_pTimer, &QTimer::timeout, this, &TimerThread::incrementAlarmDevAllTimer);
	m_pTimer->start();
	QThread::run();
}

void TimerThread::end()
{
}

TimerThread::~TimerThread()
{

}

void TimerThread::quit()
{
	m_pTimer->deleteLater();
	QThread::quit();
}

void TimerThread::incrementAlarmDevAllTimer()
{
	AlarmDev::incrementAllTimer();
	if(Const::TCP_CHECK_INTERVAL == ++m_iTcpTimer)
	{
		TcpSocket::incrementAllTimer();
		m_iTcpTimer = 0;
	}
}

