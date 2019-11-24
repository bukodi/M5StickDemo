#ifndef _UTIL_H /* include guards */
#define _UTIL_H

#include <M5StickC.h>

class Error {    
    public:
    Error(long err = 0) {
        
    }
};

void dumpHex( void *pBuff, int dataLen );

#endif /* _UTIL_H */