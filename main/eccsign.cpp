#include <M5StickC.h>
#include "mbedtls/pk.h"
#include "eccsign.h"
#include "settings.h"
#include "ecckey.h"
#include "util.h"

char *messageBuff; // Contains non-null if a sign requested for a message
unsigned char messageHash[32];
char *signBuff; // Contains nullptr if waiting for signature

void setSignRequest( const char *message ) {    
    free( messageBuff );
    free( signBuff );

    messageBuff = (char *) malloc( strlen( message )  + 1);
    strcpy( messageBuff, message );
}

int getSignature( char *message, int msgLen, char *sign, int signLen ) {
    int err = 0;

    if( messageBuff == nullptr ) {
        return ERR_NO_SIGN_REQUEST;
    }

    if( strlen( messageBuff) +1 < msgLen ) {
        return ERR_BUFFER_TOO_SMALL;
    } else {
        strcpy( message, messageBuff );
    }

    if( signBuff == nullptr ) {
        return ERR_WAITING_FOR_SIGN;
    } 

    if( strlen( signBuff) + 1 < signLen) {
        return ERR_BUFFER_TOO_SMALL;
    } else {
        strcpy( sign, signBuff );
    }

    return err;
}

void ECCSignScreen::onSetup()
{
    startUITimer(100, true);
}

void ECCSignScreen::onLongPress()
{
    if( messageBuff == nullptr ) {
        return;
    }

    if( signBuff != nullptr ) {
        return;
    }

    mbedtls_pk_context key;
    loadKeyPair( &key );
    Serial.printf("Keypair loaded:\r\n");

    hashData( messageBuff, strlen( messageBuff), messageHash );
    Serial.printf("Hash completed.\r\n");
    dumpHex( messageHash, 32);

    size_t signBytesLen = 512;
    unsigned char *signBytes = (unsigned char*)malloc(signBytesLen);
    
    signHash( &key, messageHash, signBytes, &signBytesLen );
    Serial.printf("Sign completed.\r\n");
    dumpHex( signBytes, signBytesLen);

    size_t outputLen;

    signBuff = (char*)malloc(outputLen * 2 + 1);

    strcpy( signBuff, "SIGNED");

    //base64_encode( signBytes, signBytesLen, signBuff );
    Serial.printf("Signature:\r\n%s\r\n", signBuff);
    //onRepaint();
}

unsigned long lastOn = millis();
unsigned long lastOff = lastOn + 1;

void ECCSignScreen::onTimerTick()
{
    if( messageBuff == nullptr || signBuff != nullptr ) {
        if( lastOn > lastOff ) {
            //M5.
        }
    } else {

    }
}

void ECCSignScreen::onRepaint()
{

    Serial.printf("onRepaint\r\n");
    clear();
    M5.Lcd.printf("\r\n");
    
    // TODO check "is ECC key generated?""

    if( messageBuff == nullptr ) {
        M5.Lcd.printf("\r\n Nothing\r\n to sign.\r\n");    
    } else if( signBuff == nullptr ) {
        M5.Lcd.printf("-- LP:Sign --\r\n");    
        M5.Lcd.printf("%s", messageBuff );    
    } else {
        M5.Lcd.printf("-- Signed: --\r\n");    
        M5.Lcd.printf("%s", messageBuff );    
    }
}

