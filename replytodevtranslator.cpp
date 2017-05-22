#include "replytodevtranslator.h"
#include "const.h"
#include "utils.h"
#include "crc16.h"
#include <iostream>

ReplyToDevTranslator::ReplyToDevTranslator()
{

}

QByteArray &ReplyToDevTranslator::translate(QByteArray &aDest, const QByteArray &aSrc)
{
    QDataStream is(aSrc);
    aDest.clear();
    QDataStream os(&aDest, QIODevice::WriteOnly);
    quint8 devType = Const::DEVTYPE_BOARD;
    quint32 devId = 0;
    quint16 cmdId = 0;
    quint16 mid = 0;
    quint16 dataLen = 0;
    is.skipRawData(1);
    is >> devId >> cmdId >> mid;
	if(Utils::isHeartbeatCmdId(cmdId))
	{
		cmdId = Const::CMD_ID_HEART_REPLY;
	}
	else if(Utils::isAlarmReportCmdId(cmdId))
	{
		cmdId = Const::CMD_ID_ALARM_REPLY;
	}
	else
	{
		return aDest;
	}
	os.writeRawData(Const::FRAME_START_FLAG, Const::CMD_FRAME_START_FLAG_SIZE);
	os << devType << devId;
    cmdId = Const::CMD_ID_HEART_REPLY;
    os << cmdId << mid << dataLen;
    quint16 crc16Checksum = Utils::modbus_crc((quint8 *)aDest.data(), aDest.size());
    os << crc16Checksum;
    Utils::logCmd(Const::REPLY_TO_DEV, aDest);
	return aDest;
}

