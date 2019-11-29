#include "screens.h"

const int ERR_NO_SIGN_REQUEST = -100;
const int ERR_WAITING_FOR_SIGN = -110;
const int ERR_SIGN_FAILED = -120;
const int ERR_NO_SIGN_KEY = -130;
const int ERR_BUFFER_TOO_SMALL = -140;

class ECCSignScreen : public Screen
    
{
    public:
    ECCSignScreen() : Screen("ECC Sign") {

    };
    void onSetup();
    void onRepaint();
    void onLongPress();
    void onTimerTick();
};

void setSignRequest( const char *message);
int getSignature( char *message, int pMsgLen, char *sign, int pSignLen );

