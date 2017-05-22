#include "utils.h"
#include "platformtranslator.h"
#include <QtCore>
#include <QtNetwork>
#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/helpers/stringhelper.h>
#include <log4cplus/helpers/sleep.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/thread/threads.h>

using namespace log4cplus;

static log4cplus::thread::Mutex mutex;

bool Utils::isAllBufangCmd(const  QByteArray &cmd )
{
	quint16 cmdId = getCmdIdFromCmd(cmd);
	return isAllBufangCmdId(cmdId);
}

bool Utils::isAllBufangRetCmd(const  QByteArray &cmd )
{
	quint16 cmdId = getCmdIdFromCmd(cmd);
	return isAllBufangRetCmdId(cmdId);
}

quint16 Utils::getCmdIdFromCmd(const  QByteArray &cmd )
{
	QDataStream is(cmd);
	is.skipRawData(5);
	quint16 cmdId = 0;
	is >> cmdId;
	return cmdId;
}

quint32 Utils::getDevIdFromCmd( const QByteArray &cmd )
{
	QDataStream is(cmd);
	is.skipRawData(1);
	quint32 devId = 0;
	is >> devId;
	return devId;
}

quint16 Utils::getMidFromCmd( const QByteArray &cmd )
{
	QDataStream is(cmd);
	is.skipRawData(7);
	quint16 mid = 0;
	is >> mid;
	return mid;
}

quint32 Utils::getDevIdFromJsonCmd( const QByteArray &cmd )
{
	QJsonDocument jsonDoc = QJsonDocument::fromJson(cmd);
	QJsonObject jsonCmdObj = jsonDoc.object();
	double dDevId = jsonCmdObj[KEY_DEV_ID].toDouble();
	return quint32(dDevId);
}

QByteArray Utils::toReadableCmd(const QByteArray &aCmd )
{
	QByteArray cmd;
	int size = 0, i = 0, j = 0;
	size = aCmd.size();
	if('f' == aCmd[i])
		j = 8;
	else
		j = 0;
	for(i = j; i < size; ++i)
	{
		cmd.append(aCmd.at(i));
		if(j + 1 == i)
		{
			cmd.append(' ');
		}
		else if(j + 9 == i)
		{
			cmd.append(' ');
		}
		else if(j + 13 == i)
		{
			cmd.append(' ');
		}
		else if(j + 17 == i)
		{
			cmd.append(' ');
		}
		else if(j + 21 == i)
		{
			cmd.append(' ');
		}
		else if(j + size - 5 == i)
		{
			cmd.append(' ');
		}
	}
	return cmd;
}


void Utils::logCmd( const char *direction, const QByteArray &cmd, CMD_FORMAT cmdFormat /*= BYTEARRAY_CMD*/ )
{
	LOG4CPLUS_DEBUG_FMT(
		Logger::getRoot(), 
		LOG4CPLUS_TEXT("%s : %s"), 
		direction, 
		CMD_FORMAT::BYTEARRAY_CMD == cmdFormat ? 
		toReadableCmd(cmd.toHex()).data() 
		: 
		cmd.data()
	);
}

void Utils::remoteWinAlarm( quint32 devId )
{
	QScopedPointer<QUdpSocket> pUdpSocket(new QUdpSocket());
	pUdpSocket->writeDatagram(QString("tcp %1").arg(devId).toUtf8(), QHostAddress::LocalHost, 4444);
}

void Utils::logByteArray( const QByteArray &byteArray, bool isRecv )
{
	LOG4CPLUS_WARN_FMT(
		Logger::getRoot(), 
		LOG4CPLUS_TEXT("%s : %s"), 
		isRecv ? "Recv" : "Send",
		toReadableByteArray(byteArray).constData()
		);
}

QByteArray Utils::toReadableByteArray( const QByteArray &byteArray )
{
	QByteArray readableArray;
	QByteArray hexArray = byteArray.toHex();
	for(int i = 0; i < hexArray.size(); ++i)
	{
		readableArray.append(hexArray.at(i));
		if(i % 2 != 0)
			readableArray.append(' ');
	}
	return readableArray;
}


