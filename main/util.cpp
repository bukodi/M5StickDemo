#include "util.h"

void dumpHex( void *pBuff, int dataLen ) {
    for( int i1 =0; i1 < dataLen; i1 += 16 ) {
        Serial.printf("%04X  ", i1);
        for( int i2 =0; i1+i2 < dataLen && i2 < 16; i2 ++ ) {
            Serial.printf("%02X ", ((unsigned char *)pBuff)[i1+i2]);
        }
        Serial.printf("\r\n");
    }
}
