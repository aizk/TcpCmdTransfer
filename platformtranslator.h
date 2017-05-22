#ifndef PLATFORMTRANSLATOR_H
#define PLATFORMTRANSLATOR_H

#include <QtCore>
#include "cmdtranslator.h"

static const char KEY_DEV_ID[] = "dev_id";
static const char KEY_CMD_ID[] = "cmd_id";
static const char KEY_BUFANG[] = "bufang";
static const char KEY_BUFANGTIME[] = "bufangtime";
static const char KEY_GPRS[] = "gprs";
static const char KEY_MINGDI[] = "mingdi";
static const char KEY_NETIO[] = "netio";
static const char KEY_SHEBEI[] = "shebei";
static const char KEY_ZIG315[] = "zig315";
static const char KEY_DEVTYPE[] = "devtype";
static const char KEY_STATUS[] = "status";
static const char KEY_ZIGBEE[] = "zigbee";
static const char KEY_ALARM_ID[] = "alarmid";
static const char KEY_ZONEID[] = "zoneid";
static const char KEY_ZONETYPE[] = "zonetype";
static const char KEY_PARAM1[] = "param1";
static const char KEY_PARAM2[] = "param2";
static const char KEY_RETURN[] = "return";

using namespace std;

struct Alarm
{
	quint8    alarmid;
	quint8    zonetype;
	quint8    zoneid;
};

struct zone_315
{
    zone_315()
        : devtype(0),
          status(0),
          bufang(0)
    {

    }
    quint8    devtype;  //设备类型：参考设备类型对应表
    quint8    status;	//防区状态：0：正常 1：低电量 2：异常报警 3：设备掉线
    quint8    bufang;	//是否布防：0：撤防  1：布防 （在报警板设为选择布撤防时标记各个防区布撤防状态）
};

struct zone_zigbee
{
    zone_zigbee()
        : devtype(0),
          param1(0),
          param2(0),
          status(0),
          bufang(0)
    {

    }
    quint8    devtype;      //设备类型：参考设备类型对应表
    quint8    param1;		//上报数据：根据设备类型定夺，温湿度类型时此数据去为温度，PM2.5采集仪时定义该数据去为PM2.5的值
    quint8    param2;		//上报数据：根据设备类型定夺，温湿度类型时此数据去为湿度，PM2.5采集仪时定义该数据区为空
    quint8    status;		//防区状态：0：正常 1：低电量 2：异常报警 3：设备掉线
    quint8    bufang;		//是否布防：0：撤防  1：布防（在报警板设为选择布撤防时标记各个防区布撤防状态）
};

struct board_status
{
    board_status()
        : netio(0),
          gprs(0),
          mingdi(0),
          shebei(0),
          bufang(0)
    {

    }
    quint8    netio;      //每块报警板上有两个网络IO口，这个值为0：全部关闭，1：1号口打开，2：	2号口打开，3：两个IO口同时打开。
    quint8    gprs;       //报警板GPRS信号：值从0~100，值越大，信号越强
    quint8    mingdi;     //鸣笛状态：1 打开  0关闭
    quint8    shebei;     // 1 充电状态   2电池供电  3低电量提醒
    quint8    bufang;     // 0：撤防  1：全部布防 2：选择布撤防
    vector<zone_zigbee> zigbee;
    vector<zone_315> zig315;
};


class PlatformTranslator : public CmdTranslator
{
public:
    PlatformTranslator(const int iDirection);

protected:
    QByteArray & toDevCmd(QByteArray &aDevCmd, const QByteArray &aSrcCmd);
    QByteArray & fromDevCmd(QByteArray &aDestCmd, const QByteArray &aDevCmd);

private:
	static quint16 curBoardCrc;
};

QDataStream &operator >>(QDataStream &s, zone_315 &zone315);
QDataStream &operator >>(QDataStream &s, zone_zigbee &zoneZigbee);
QDataStream &operator >>(QDataStream &s, board_status &boardStatus);
QDataStream &operator >>(QDataStream &s, Alarm &alarm);

QDataStream &operator <<(QDataStream &s, const zone_315 &zone315);
QDataStream &operator <<(QDataStream &s, const zone_zigbee &zoneZigbee);
QDataStream &operator <<(QDataStream &s, const board_status &boardStatus);

int toJsonObject(QJsonObject &destJson, const zone_315 &zone315);
int toJsonObject(QJsonObject &destJson, const zone_zigbee &zoneZigbee);
int toJsonObject(QJsonObject &destJson, const board_status &boardStatus);
int toJsonObject(QJsonObject &destJson, const Alarm &alarm);

int jsonCmdRetToPlatform(
        QJsonObject &destJson,
        quint32 devId,
        quint16 cmdId,
        int returnCode
        );



#endif // PLATFORMTRANSLATOR_H
