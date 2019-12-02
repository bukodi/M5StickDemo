#include <M5StickC.h>
#include "mbedtls/pk.h"
#include "eccsign.h"
#include "settings.h"
#include "ecckey.h"
#include "util.h"

ECCSignScreen eccSignScreen;

String messageToSign = "";
String messageHash = "";
String signature = "";

void setSignRequest( String message ) {    
    signature = "";

    Serial.printf("Before set messageBuff=%s\n", message);
    messageToSign = message;
    Serial.printf("After set messageBuff=%s\n", messageToSign);
    // Turn LED on
    digitalWrite (10, LOW);
    eccSignScreen.activate();    
    eccSignScreen.onRepaint();
}

int getSignature( String& retMsg, String& retSign ) {
    int err = 0;

    if( messageToSign.length() == 0 ) {
        retMsg = "No pending message.";
        retSign =  "No signature.";
        return ERR_NO_SIGN_REQUEST;
    }
    retMsg = messageToSign;

    if( signature.length() == 0 ) {
        retSign =  "Waiting for signature.";
        return ERR_WAITING_FOR_SIGN;
    }
    retSign = signature;

    return err;
}

void ECCSignScreen::onSetup()
{
    startUITimer(100, true);
}

void ECCSignScreen::onLongPress()
{
    if( messageToSign.length() == 0 ) {
        return;
    }

    if( signature.length() > 0 ) {
        return;
    }
    Serial.printf("---onLongPress---\n");
    Serial.printf("Message buff:%s\n", messageToSign.c_str());

    mbedtls_pk_context key;
    loadKeyPair( &key );
    Serial.printf("Keypair loaded:\r\n");

    unsigned char messageHash[32];
    hashData( messageToSign.c_str(), messageToSign.length(), messageHash );
    Serial.printf("Hash completed.\r\n");
    dumpHex( messageHash, 32);

    size_t signBytesLen = 512;
    unsigned char *signBytes = (unsigned char*)malloc(signBytesLen);
    
    signHash( &key, messageHash, signBytes, &signBytesLen );
    Serial.printf("Sign completed.\r\n");
    dumpHex( signBytes, signBytesLen);

    signature = base64_encode( signBytes, signBytesLen);

    digitalWrite (10, HIGH);
    onRepaint();
}

unsigned long lastOn = millis();
unsigned long lastOff = lastOn + 1;

void ECCSignScreen::onTimerTick()
{
    if( messageToSign.length() > 0 && signature.length() == 0 ) {
        unsigned long now = millis();
        if( lastOn > lastOff && lastOn + 200 < now) {
            digitalWrite (10, HIGH);
            lastOff = now;
        } else if( lastOff > lastOn && lastOff + 500 < now ) {
            digitalWrite (10, LOW);
            lastOn = now;
        } else {
            // Nothing to do
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

    if( messageToSign.length() == 0 ) {
        M5.Lcd.printf("\r\n Nothing\r\n to sign.\r\n");    
    } else if( signature.length() == 0 ) {
        M5.Lcd.printf("-- LP:Sign --\r\n");    
        M5.Lcd.printf("%s", messageToSign.c_str() );    
    } else {
        M5.Lcd.printf("-- Signed: --\r\n");    
        M5.Lcd.printf("%s", messageToSign.c_str() );    
    }
}

