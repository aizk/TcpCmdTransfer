#include "nochangetranslator.h"
#include "utils.h"
#include <iostream>

NoChangeTranslator::NoChangeTranslator(const int iDirection)
    : CmdTranslator(iDirection)
{

}



QByteArray &NoChangeTranslator::toDevCmd(QByteArray &aDevCmd, const QByteArray &aSrcCmd)
{
    Utils::logCmd(Const::APP_TO_DEV, aSrcCmd);
	aDevCmd = aSrcCmd;
	aDevCmd.push_front(QByteArray(Const::FRAME_START_FLAG, Const::CMD_FRAME_START_FLAG_SIZE));
    return aDevCmd;
}

QByteArray &NoChangeTranslator::fromDevCmd(QByteArray &aDestCmd, const QByteArray &aDevCmd)
{
	Utils::logCmd(Const::TRANSLATE_TO_APP, aDevCmd);
    aDestCmd = aDevCmd;
	aDestCmd.push_front(QByteArray(Const::FRAME_START_FLAG, Const::CMD_FRAME_START_FLAG_SIZE));
    return aDestCmd;
}

