#include "platformtranslator.h"
#include "utils.h"
#include "crc16.h"
#include "alarmdev.h"
#include <iostream>

quint16 PlatformTranslator::curBoardCrc = 0;

PlatformTranslator::PlatformTranslator(const int iDirection)
    : CmdTranslator(iDirection)
{

}

QByteArray &PlatformTranslator::toDevCmd(QByteArray &aDevCmd, const QByteArray &aSrcCmd)
{
    QJsonDocument srcJsonDoc = QJsonDocument::fromJson(aSrcCmd);
    QJsonObject srcJsonObj = srcJsonDoc.object();
    quint32 devId = srcJsonObj[KEY_DEV_ID].toDouble();
    quint16 cmdId = srcJsonObj[KEY_CMD_ID].toInt();
	quint16 mid = 0;
	quint16 dataLen = 0;
	quint8 devType = 0x01;
    QDataStream os(&aDevCmd, QIODevice::WriteOnly);
    if(Utils::isAllBufangCmdId(cmdId) || Utils::isAllCFangCmdId(cmdId))
    {
		os.writeRawData(Const::FRAME_START_FLAG, Const::CMD_FRAME_START_FLAG_SIZE);
		os 	<< devType 
			<< devId 
			<< cmdId 
			<< mid 
			<< dataLen;
    }
    quint16 crc16Checksum = Utils::modbus_crc((quint8 *)aDevCmd.data(), aDevCmd.size());
    os << crc16Checksum;
	Utils::logCmd(Const::PLATFORM_TO_DEV_ARRAY, aDevCmd);
    return aDevCmd;
}

QByteArray &PlatformTranslator::fromDevCmd(QByteArray &aDestCmd, const QByteArray &aDevCmd)
{
	QDataStream  is(aDevCmd);
    is.skipRawData(1);
    quint32 devId = 0;
    quint16 cmdId = 0;
	is >> devId >> cmdId;
	is.skipRawData(4); // Ìø¹ýMIDºÍDataLen
	QJsonObject destJsonObj;
	if(Utils::isAllBufangRetCmdId(cmdId) 
		|| Utils::isAllCFangCmdId(cmdId))
	{
		destJsonObj[KEY_RETURN] = 0;
	}
	else if(Utils::isAlarmReportCmdId(cmdId))
	{
		Alarm alarm;
		is >> alarm;
		toJsonObject(destJsonObj, alarm);
	}
	else if(Utils::isHeartbeatCmdId(cmdId))
	{
		AlarmDev *pAlarmDev = AlarmDev::getAlarmDevByDevId(devId);
		quint16 crcCheck = Utils::modbus_crc((quint8 *)aDevCmd.data(), aDevCmd.size());
		if(!pAlarmDev->isCrcChanged(crcCheck))
		{
			aDestCmd.clear();
			return aDestCmd;
		}
		pAlarmDev->setCrc(crcCheck);
		board_status boardStatus;
		is >> boardStatus;
		toJsonObject(destJsonObj, boardStatus);
		if(pAlarmDev && boardStatus.bufang != pAlarmDev->bufang)
		{
			qint64 curTimestamp = QDateTime::currentDateTimeUtc().toTime_t();
			destJsonObj[KEY_BUFANGTIME] = double(curTimestamp);
			pAlarmDev->bufang = boardStatus.bufang;
		}
	}
	else if(Utils::isOnlineCmdId(cmdId))
	{
		quint8 online = 0;
		is >> online;
		destJsonObj[KEY_STATUS] = online;
	}
    destJsonObj[KEY_DEV_ID] = double(devId);
    destJsonObj[KEY_CMD_ID] = cmdId;
    aDestCmd = QJsonDocument(destJsonObj).toJson(QJsonDocument::Compact);
	Utils::logCmd(Const::DEV_TO_PLATFORM_JSON, aDestCmd, Utils::JSON_CMD);
    return aDestCmd;
}

QDataStream &operator >>(QDataStream &s, zone_315 &zone315)
{
    s >> zone315.devtype;
    s >> zone315.status;
    s >> zone315.bufang;
    return s;
}

QDataStream &operator >>(QDataStream &s, zone_zigbee &zoneZigbee)
{
    s >> zoneZigbee.devtype;
    s >> zoneZigbee.param1;
    s >> zoneZigbee.param2;
    s >> zoneZigbee.status;
    s >> zoneZigbee.bufang;
    return s;
}

QDataStream &operator >>(QDataStream &s, board_status &boardStatus)
{
    s >> boardStatus.netio;
    s >> boardStatus.gprs;
    s >> boardStatus.mingdi;
    s >> boardStatus.shebei;
    s >> boardStatus.bufang;
    quint8 cZigbee = 0;
    int i = 0;
    zone_zigbee tempZigbee;
    s >> cZigbee;
    for(i = 0; i < cZigbee; ++i)
    {
        s >> tempZigbee;
        boardStatus.zigbee.push_back(tempZigbee);
    }
    quint8 cZone315 = 0;
    zone_315 tempZone315;
    s >> cZone315;
    for(i = 0; i < cZone315; ++i)
    {
        s >> tempZone315;
        boardStatus.zig315.push_back(tempZone315);
    }
    return s;
}

QDataStream & operator>>( QDataStream &s, Alarm &alarm )
{
	s >> alarm.alarmid;
	s >> alarm.zonetype;
	s >> alarm.zoneid;
	return s;
}

QDataStream &operator <<(QDataStream &s, const zone_315 &zone315)
{
    s << zone315.devtype;
    s << zone315.status;
    s << zone315.bufang;
    return s;
}

QDataStream &operator <<(QDataStream &s, const zone_zigbee &zoneZigbee)
{
    s << zoneZigbee.devtype;
    s << zoneZigbee.param1;
    s << zoneZigbee.param2;
    s << zoneZigbee.status;
    s << zoneZigbee.bufang;
    return s;
}

QDataStream &operator <<(QDataStream &s, const board_status &boardStatus)
{
    s << boardStatus.netio;
    s << boardStatus.gprs;
    s << boardStatus.mingdi;
    s << boardStatus.shebei;
    s << boardStatus.bufang;
    int cZigbee = 0;
    int i = 0;
    cZigbee = boardStatus.zigbee.size();
    s << cZigbee;
    for(i = 0; i < cZigbee; ++i)
    {
        s << boardStatus.zigbee[i];
    }
    int cZone315 = boardStatus.zig315.size();
    s << cZone315;
    for(i = 0; i < cZone315; ++i)
    {
        s << boardStatus.zig315[i];
    }
    return s;
}

int toJsonObject(QJsonObject &destJson, const zone_315 &zone315)
{
    destJson[KEY_DEVTYPE] = zone315.devtype;
    destJson[KEY_STATUS] = zone315.status;
    destJson[KEY_BUFANG] = zone315.bufang;
    return 0;
}

int toJsonObject(QJsonObject &destJson, const zone_zigbee &zoneZigbee)
{
    destJson[KEY_DEVTYPE] = zoneZigbee.devtype;
    destJson[KEY_PARAM1] = zoneZigbee.param1;
    destJson[KEY_PARAM2] = zoneZigbee.param2;
    destJson[KEY_STATUS] = zoneZigbee.status;
    destJson[KEY_BUFANG] = zoneZigbee.bufang;
    return 0;
}

int toJsonObject(QJsonObject &destJson, const board_status &boardStatus)
{
    destJson[KEY_NETIO] = boardStatus.netio;
    destJson[KEY_GPRS] = boardStatus.gprs;
    destJson[KEY_MINGDI] = boardStatus.mingdi;
    destJson[KEY_SHEBEI] = boardStatus.shebei;
    destJson[KEY_BUFANG] = boardStatus.bufang;
    int cZig = boardStatus.zigbee.size();
	int i = 0;
    QJsonArray tempJsonArray;
	QJsonObject tempJsonObj;
    for(i = 0; i < cZig; ++i)
    {
        tempJsonObj = QJsonObject();
        toJsonObject(tempJsonObj, boardStatus.zigbee[i]);
        tempJsonArray.append(QJsonValue(tempJsonObj));
    }
    destJson[KEY_ZIGBEE] = tempJsonArray;
	cZig = boardStatus.zig315.size();
	for(i = 0; i < cZig; ++i)
	{
		tempJsonObj = QJsonObject();
		toJsonObject(tempJsonObj, boardStatus.zig315[i]);
		tempJsonArray.append(QJsonValue(tempJsonObj));
	}
	destJson[KEY_ZIG315] = tempJsonArray;
	return 0;
}

int toJsonObject( QJsonObject &destJson, const Alarm &alarm )
{
	destJson[KEY_ALARM_ID] = alarm.alarmid;
	destJson[KEY_ZONETYPE] = alarm.zonetype;
	destJson[KEY_ZONEID] = alarm.zoneid;
	return 0;
}

int jsonCmdRetToPlatform(
        QJsonObject &destJson,
        quint32 devId,
        quint16 cmdId,
        int returnCode)
{
    destJson[KEY_DEV_ID] = double(devId);
    destJson[KEY_CMD_ID] = cmdId;
    destJson[KEY_RETURN] = returnCode;
	return 0;
}

