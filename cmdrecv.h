#ifndef CMDRECV_H
#define CMDRECV_H

#include "icmdrecv.h"
#include "cmdfactory.h"

class CmdRecv : public ICmdRecv
{
public:
    CmdRecv(CmdFactory *pCmdFactory);

protected:
    CmdFactory *m_pCmdFactory;
};

#endif // CMDRECV_H
