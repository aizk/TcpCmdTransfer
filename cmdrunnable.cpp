#include "cmdrunnable.h"

CmdRunnable::CmdRunnable( Cmd *pCmd )
{
    m_pCmd = pCmd;
}

CmdRunnable::~CmdRunnable()
{
    delete m_pCmd;
}

void CmdRunnable::run()
{
    m_pCmd->handle();
}

