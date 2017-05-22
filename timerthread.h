#ifndef TIMERTHREAD_H
#define TIMERTHREAD_H

#include <QtCore>

class TimerThread : public QThread
{
public:
    TimerThread();
    ~TimerThread();

    void run();
    void end();
	void quit();

protected:
	void incrementAlarmDevAllTimer();

private:
    volatile bool m_bStartToEnd;
    //QElapsedTimer *m_pTimer;
	QTimer *m_pTimer;
	volatile quint64 m_iTcpTimer;
};

#endif // TIMERTHREAD_H
