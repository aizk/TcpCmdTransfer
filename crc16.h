#ifndef CRC16_H
#define CRC16_H

#include <QtCore>

typedef quint8 MB_U8;
typedef quint16 MB_U16;

namespace Utils {
    quint16 modbus_crc(MB_U8 *p, int n);
}


#endif // CRC16_H

