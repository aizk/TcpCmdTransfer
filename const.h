#pragma warning(once:4309) // ֻ��һ�θþ��棬warning C4309: ����ʼ����: �ضϳ���ֵ

#ifndef CONST_H
#define CONST_H

namespace Const {

	const int RTN_ERROR = -1;									// ���ش���ֵ
    const int DEVTYPE_BOARD = 0x01;								// �������豸����
    const int CMD_BUFFER_SIZE = 1024;							// ָ�������󳤶�
	const int CMD_FRAME_START_FLAG_SIZE = 4;					// ָ����ʼ��ǳ��ȣ�4��0xFF
    const int DEV_TYPE_SIZE = 1;								// �豸���Ͷγ���
    const int DEV_ID_SIZE = 4;									// �豸ID�γ���
    const int CMD_ID_SIZE = 2;									// ָ��ID�γ���
    const int MID_SIZE = 2;										// MID�γ���
    const int LEN_SIZE = 2;										// ���ݳ��ȶγ���
    const int CRC_SIZE = 2;										// CRCУ���볤��
	// ָ��֡ͷ���ȣ�����
	// ָ����ʼ��ǳ��ȣ��豸���Ͷγ��ȣ��豸ID�γ��ȣ�ָ��ID�γ��ȣ�MID�γ��ȣ����ݳ��ȶγ��ȣ�CRCУ���볤��
	const int CMD_HEAD_SIZE = CMD_FRAME_START_FLAG_SIZE + DEV_TYPE_SIZE + DEV_ID_SIZE + CMD_ID_SIZE + MID_SIZE + LEN_SIZE;
    const int MIN_CMD_SIZE = CMD_HEAD_SIZE + CRC_SIZE;			// ��Сָ���
	const int MAX_CMD_LEN = 250;								// ���ָ���
	const char FRAME_START_FLAG_BYTE = 0xFF;					// ֡��ʼ���ֽ�
	const char FRAME_START_FLAG[] = {0xFF, 0xFF, 0xFF, 0xFF};	// ֡��ʼ��ʶ��4��0xFF

	const int CMD_ID_ALL_BUFANG = 0x0700;						// ����ID - ȫ������
	const int CMD_ID_ALL_BUFANG_RTN = 0x8700;					// ����ID - ȫ�������ظ�
	const int CMD_ID_ALL_CFANG = 0x0702;						// ����ID - ȫ������
	const int CMD_ID_ALL_CFANG_RTN = 0x8702;					// ����ID - ȫ�������ظ�
	const int CMD_ID_ALARM_REPORT = 0x8801;						// ����ID - �����ϱ�
	const int CMD_ID_ALARM_REPLY = 0x0801;						// ����ID - ����ظ�
	const int CMD_ID_HEARTBEAT = 0x8800;						// ����ID - �豸����
    const int CMD_ID_HEART_REPLY = 0x0800;						// ����ID - �豸�����ظ�
	const int CMD_ID_APP_HEART = 0x8802;						// ����ID - APP����
    const int CMD_ID_APP_HEART_REPLY = 0x0802;					// ����ID - APP�����ظ�
	const int CMD_ID_ONLINE = 0x8804;							// ����ID - ������豸����ָ���ƽ̨��ʶ���豸�Ƿ������
	const int CMD_ID_RECV_DATA_ERROR = 0xff00;					// ����ID - �豸�������ݴ���

	const int CMD_SECCESS = 0;									// �·���ָ��ִ�гɹ�
	const int CMD_TIMEOUT = 1;									// �·���ָ�ʱ
	const int CMD_DEV_NOT_EXIST = 3;							// �豸������
	const int CMD_DEV_OFFLINE = 4;								// �豸����

	const int MID_TO_APP = 0xffff;								// ֱ�Ӵ��豸���͵�APP�˵�ָ���MIDֵ
	const int TIMER_INTERVAL = 1000;							// �����л�����ʱ����ʱ����Ϊ1s
	const int ONE_SECOND = 1;									// 1��
	const int ONE_MINUTE = 60 * ONE_SECOND;						// 1����
	const int NO_HEART_TIME = ONE_MINUTE * 5;					// 5���������������жϵ���
	const int NO_CMD_RTN_TIME = ONE_SECOND * 6;					// 6����ָ��ظ������ж�ָ�ʱ
	const int TCP_CHECK_INTERVAL = ONE_MINUTE;					// ���TcpSocket�������ݵ�ʱ������1����
	const int TCP_NO_DATA_TIME = 5;								// 5���������ݣ����ж�TcpSocket����

	const int MAX_TCP_THREAD = 3;								// ����APP���豸�����߳���

	const char CONFIG_FILE_NAME[] = "app.ini"; // ���������ļ���

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

