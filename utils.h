#ifndef UTILS_H
#define UTILS_H

#include <QtCore>
#include "const.h"

namespace Utils {

	typedef enum {
		BYTEARRAY_CMD,
		JSON_CMD
	}CMD_FORMAT;


	inline bool isAllBufangCmdId(quint16 cmdId)
	{
		return Const::CMD_ID_ALL_BUFANG == cmdId;
	}

	inline bool isAllBufangRetCmdId(quint16 cmdId)
	{
		return Const::CMD_ID_ALL_BUFANG_RTN == cmdId;
	}

	inline bool isAllCFangCmdId(quint16 cmdId)
	{
		return Const::CMD_ID_ALL_CFANG == cmdId;
	}

	inline bool isAllCFangRetCmdId(quint16 cmdId)
	{
		return Const::CMD_ID_ALL_CFANG_RTN == cmdId;
	}


	inline bool isHeartbeatCmdId(quint16 cmdId)
	{
		return Const::CMD_ID_HEARTBEAT == cmdId;
	}

	inline bool isAlarmReportCmdId(quint16 cmdId)
	{
		return Const::CMD_ID_ALARM_REPORT == cmdId;
	}

	inline bool isAlarmConfirmCmdId(quint16 cmdId)
	{
		return 0x0801 == cmdId;
	}

	inline bool isOnlineCmdId(quint16 cmdId)
	{
		return Const::CMD_ID_ONLINE == cmdId;
	}

	inline bool isAppHeart(quint16 cmdId)
	{
		return Const::CMD_ID_APP_HEART == cmdId;
	}

	inline bool isAppHeartReply(quint16 cmdId)
	{
		return Const::CMD_ID_APP_HEART_REPLY == cmdId;
	}

	inline bool isRecvDataError(quint16 cmdId)
	{
		return Const::CMD_ID_RECV_DATA_ERROR == cmdId;
	}

	bool isAllBufangCmd(const QByteArray &cmd);
	bool isAllBufangRetCmd(const QByteArray &cmd);

	quint16 getCmdIdFromCmd(const QByteArray &cmd);
	quint16 getMidFromCmd(const QByteArray &cmd);
	quint32 getDevIdFromCmd(const QByteArray &cmd);
	quint32 getDevIdFromJsonCmd(const QByteArray &cmd);

	inline quint64 toIpPort(quint32 ipAddr, quint16 port)
	{
		quint64 ipPort = ipAddr;
		ipPort = ipPort << 16;
		ipPort += port;
		return ipPort;
	}

	QByteArray toReadableCmd(const QByteArray &aCmd);

	void logCmd(const char *direction, const QByteArray &cmd, CMD_FORMAT cmdFormat = BYTEARRAY_CMD);
	void remoteWinAlarm(quint32 devId);
	void logByteArray(const QByteArray &byteArray, bool isRecv);
	QByteArray toReadableByteArray(const QByteArray &byteArray);
}

#endif // UTILS_H
