#ifndef _UTIL_H /* include guards */
#define _UTIL_H

#include <M5StickC.h>

class Error {    
    public:
    Error(long err = 0) {
        
    }
};

void dumpHex( void *pBuff, int dataLen );

void base64_encode(const unsigned char *data, size_t input_length, char *output );
unsigned char *base64_decode(const char *data, size_t input_length, size_t *output_length);


#endif /* _UTIL_H */