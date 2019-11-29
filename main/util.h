#ifndef _UTIL_H /* include guards */
#define _UTIL_H

#include <M5StickC.h>

class Error {    
    public:
    Error(long err = 0) {
        
    }
};

void dumpHex( void *pBuff, int dataLen );

String base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len);
//unsigned char *base64_decode(const char *data, size_t input_length, size_t *output_length);


#endif /* _UTIL_H */