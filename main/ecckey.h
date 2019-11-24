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

