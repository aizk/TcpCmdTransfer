#ifndef CMDRUNNABLE_H
#define CMDRUNNABLE_H

#include <QtCore>
#include "cmd.h"

class CmdRunnable : public QRunnable
{
public:
	CmdRunnable(Cmd *pCmd);
    ~CmdRunnable();
    void run();

private:
    Cmd *m_pCmd;
};

#endif // CMDRUNNABLE_H
