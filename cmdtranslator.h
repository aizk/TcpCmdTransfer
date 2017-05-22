#ifndef CMDTRANSLATOR_H
#define CMDTRANSLATOR_H

#include "icmdtranslator.h"

class CmdTranslator : public ICmdTranslator
{
public:
    enum TranslateDirection
    {
        FROM_DEV,
        TO_DEV
    };

    CmdTranslator(const int iDirection);
    QByteArray getTranslatedData();
    void setDataArea(const QByteArray &dataArea);
    void setDevId(quint32 iDevId);
    void setCmdId(quint16 iCmdId);
    void setDirection(int iDirection);
    QByteArray & translate(QByteArray &aDest, const QByteArray &aSrc);

protected:
    virtual QByteArray & toDevCmd(QByteArray &aDevCmd, const QByteArray &aSrcCmd) = 0;
    virtual QByteArray & fromDevCmd(QByteArray &aDestCmd, const QByteArray &aDevCmd) = 0;

protected:
    QByteArray m_translatedData;
    quint32 m_devId;
    quint16 m_cmdId;
    QByteArray m_dataArea;
    int m_iDirection;
};

#endif // CMDTRANSLATOR_H
