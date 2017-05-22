#include "cmdtranslator.h"

CmdTranslator::CmdTranslator(const int iDirection)
    : m_devId(0),
      m_cmdId(0),
      m_iDirection(iDirection)
{

}

QByteArray & CmdTranslator::translate(QByteArray &aDest, const QByteArray &aSrc)
{
    if(FROM_DEV == m_iDirection)
        return fromDevCmd(aDest, aSrc);
    else
        return toDevCmd(aDest, aSrc);
}

QByteArray CmdTranslator::getTranslatedData()
{
    return m_translatedData;
}

void CmdTranslator::setDataArea(const QByteArray &dataArea)
{
    m_dataArea = dataArea;
}

void CmdTranslator::setDevId(quint32 iDevId)
{
    m_devId = iDevId;
}

void CmdTranslator::setCmdId(quint16 iCmdId)
{
    m_cmdId = iCmdId;
}

void CmdTranslator::setDirection(int iDirection)
{
    m_iDirection = iDirection;
}


