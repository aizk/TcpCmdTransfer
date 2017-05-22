#pragma warning(once:4309) // 只报一次该警告，warning C4309: “初始化”: 截断常量值

#ifndef CONST_H
#define CONST_H

namespace Const {

	const int RTN_ERROR = -1;									// 返回错误值
    const int DEVTYPE_BOARD = 0x01;								// 报警板设备类型
    const int CMD_BUFFER_SIZE = 1024;							// 指令缓冲区最大长度
	const int CMD_FRAME_START_FLAG_SIZE = 4;					// 指令起始标记长度，4个0xFF
    const int DEV_TYPE_SIZE = 1;								// 设备类型段长度
    const int DEV_ID_SIZE = 4;									// 设备ID段长度
    const int CMD_ID_SIZE = 2;									// 指令ID段长度
    const int MID_SIZE = 2;										// MID段长度
    const int LEN_SIZE = 2;										// 数据长度段长度
    const int CRC_SIZE = 2;										// CRC校验码长度
	// 指令帧头长度，包括
	// 指令起始标记长度，设备类型段长度，设备ID段长度，指令ID段长度，MID段长度，数据长度段长度，CRC校验码长度
	const int CMD_HEAD_SIZE = CMD_FRAME_START_FLAG_SIZE + DEV_TYPE_SIZE + DEV_ID_SIZE + CMD_ID_SIZE + MID_SIZE + LEN_SIZE;
    const int MIN_CMD_SIZE = CMD_HEAD_SIZE + CRC_SIZE;			// 最小指令长度
	const int MAX_CMD_LEN = 250;								// 最大指令长度
	const char FRAME_START_FLAG_BYTE = 0xFF;					// 帧起始的字节
	const char FRAME_START_FLAG[] = {0xFF, 0xFF, 0xFF, 0xFF};	// 帧起始标识，4个0xFF

	const int CMD_ID_ALL_BUFANG = 0x0700;						// 命令ID - 全部布防
	const int CMD_ID_ALL_BUFANG_RTN = 0x8700;					// 命令ID - 全部布防回复
	const int CMD_ID_ALL_CFANG = 0x0702;						// 命令ID - 全部撤防
	const int CMD_ID_ALL_CFANG_RTN = 0x8702;					// 命令ID - 全部撤防回复
	const int CMD_ID_ALARM_REPORT = 0x8801;						// 命令ID - 警情上报
	const int CMD_ID_ALARM_REPLY = 0x0801;						// 命令ID - 警情回复
	const int CMD_ID_HEARTBEAT = 0x8800;						// 命令ID - 设备心跳
    const int CMD_ID_HEART_REPLY = 0x0800;						// 命令ID - 设备心跳回复
	const int CMD_ID_APP_HEART = 0x8802;						// 命令ID - APP心跳
    const int CMD_ID_APP_HEART_REPLY = 0x0802;					// 命令ID - APP心跳回复
	const int CMD_ID_ONLINE = 0x8804;							// 命令ID - 虚拟的设备上线指令，由平台来识别设备是否刚上线
	const int CMD_ID_RECV_DATA_ERROR = 0xff00;					// 命令ID - 设备接收数据错误

	const int CMD_SECCESS = 0;									// 下发的指令执行成功
	const int CMD_TIMEOUT = 1;									// 下发的指令超时
	const int CMD_DEV_NOT_EXIST = 3;							// 设备不存在
	const int CMD_DEV_OFFLINE = 4;								// 设备离线

	const int MID_TO_APP = 0xffff;								// 直接从设备发送到APP端的指令的MID值
	const int TIMER_INTERVAL = 1000;							// 程序中基础计时器的时间间隔为1s
	const int ONE_SECOND = 1;									// 1秒
	const int ONE_MINUTE = 60 * ONE_SECOND;						// 1分种
	const int NO_HEART_TIME = ONE_MINUTE * 5;					// 5分钟无心跳，则判断掉线
	const int NO_CMD_RTN_TIME = ONE_SECOND * 6;					// 6秒无指令回复，则判断指令超时
	const int TCP_CHECK_INTERVAL = ONE_MINUTE;					// 检测TcpSocket有无数据的时间间隔，1分钟
	const int TCP_NO_DATA_TIME = 5;								// 5分钟无数据，则中断TcpSocket连接

	const int MAX_TCP_THREAD = 3;								// 最大的APP或设备连接线程数

	const char CONFIG_FILE_NAME[] = "app.ini"; // 程序配置文件名

	const char DATETIME_FORMT[] = "yyyy-MM-dd HH:mm:ss:zzz";
	const char FROM_DEV[] = "from Dev";
	const char FROM_APP[] = "from App";
	const char TRANSLATE_TO_APP[] = "Translate to App";
	const char FROM_PLATFORM[] = "from Platform";
	const char HTTP_HANDLER[] = " HTTPHandler";
	const char HTTPPOST_OK[] = "HttpPost OK";
	const char APP_TO_DEV[] = "App to Dev";
	const char DEV_TO_PLATFORM_ARRAY[] = "Dev to Platform ByteArray";
	const char DEV_TO_PLATFORM_JSON[] = "Dev to Platform Json";
	const char PLATFORM_TO_DEV_ARRAY[] = "Platform to Dev Array";
	const char REPLY_TO_DEV[] = "reply to Dev";
	const char STR_DEV[] = "Dev";
	const char STR_APP[] = "App";
	

}

#endif // CONST_H

