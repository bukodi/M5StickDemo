#include <M5StickC.h>
#include "mbedtls/pk.h"
#include "eccsign.h"
#include "settings.h"
#include "ecckey.h"

char *messageBuff; // Contains non-null if a sign requested for a message
unsigned char messageHash[32];
char *signBuff; // Contains nullptr if waiting for signature

void setSignRequest( char *message ) {    
    free( messageBuff );
    free( signBuff );

    messageBuff = (char *) malloc( strlen( message )  + 1);
    strcpy( messageBuff, message );
}

int getSignature( char *message, int msgLen, char *sign, int *signLen ) {
    int err = 0;

    if( messageBuff == nullptr ) {
        return ERR_NO_SIGN_REQUEST;
    }

    if( strlen( messageBuff) +1 < msgLen ) {
        return ERR_BUFFER_TOO_SMALL;
    }

    if( signBuff == nullptr ) {
        return ERR_WAITING_FOR_SIGN;
    }

    mbedtls_pk_context key;
    if( err = loadKeyPair( &key ) ) {
        return err;
    }

    hashData( messageBuff, strlen( messageBuff), messageHash );

    size_t signBuffLen = 256;
    unsigned char signBuff[256];
    
    signHash( &key, messageHash, signBuff, &signBuffLen );

    
    return err;
}

void ECCSignScreen::onSetup()
{
    startUITimer(100, true);
}

void ECCSignScreen::onLongPress()
{
}

void ECCSignScreen::onTimerTick()
{
    //if( messageBuff == null )
}

void ECCSignScreen::onRepaint()
{

    clear();
    M5.Lcd.printf("\r\n");
    
    int genCount = 0;
    ConfigGetIntValue("ECCGenCount", &genCount, 0);
    M5.Lcd.printf("Gen count:%d\r\n", genCount);
    
    mbedtls_pk_context key;
    if( loadKeyPair( &key ) ) {
        M5.Lcd.printf("\r\n");
        M5.Lcd.printf("Key not exists.\r\n", genCount);        
        M5.Lcd.printf("\r\nLong press:\r\n");
        M5.Lcd.printf("Generate new key\r\n");
    } else {
        unsigned char hashBuff[32];
        hashPublicKey( &key, hashBuff);

        M5.Lcd.printf("\r\nPub key hash:\r\n");
        for( int i1 =0; i1 < 32; i1 += 4 ) {
            M5.Lcd.printf("  %02X%02X%02X%02X\r\n", hashBuff[i1], hashBuff[i1+1], hashBuff[i1+2], hashBuff[i1+3] );
        }
        M5.Lcd.printf("\r\nLong press:\r\n");
        M5.Lcd.printf("Generate new key\r\n");
    }
}

