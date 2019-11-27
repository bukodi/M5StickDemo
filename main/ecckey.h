#include "screens.h"
#include "mbedtls/pk.h"

class ECCKeyScreen : public Screen
    
{
    public:
    ECCKeyScreen() : Screen("ECC Key") {

    };
    void onSetup();
    void onRepaint();
    void onLongPress();
};

int getPublicKeyPEM( char *buff, size_t buffLen );
int loadKeyPair( mbedtls_pk_context *key );
void hashData( const void *message, int dataLen, unsigned char* hashBuff);
void signHash( mbedtls_pk_context *key, const unsigned char *hash, unsigned char *pBuff, size_t *buffSize );

