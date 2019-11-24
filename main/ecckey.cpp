#include <M5StickC.h>
#include "ecckey.h"
#include "mbedtls/x509_crt.h"
#include "mbedtls/ecp.h"
#include "mbedtls/ecdsa.h"
#include "mbedtls/pk.h"
#include "mbedtls/pk_internal.h"
#include "settings.h"
#include "util.h"

int genCount = 0;

static int myrand(void *rng_state, unsigned char *output, size_t len);

void generateECKeyPair()
{
    int err;
    mbedtls_ecdsa_context ecKeyPair;
    mbedtls_ecdsa_init(&ecKeyPair);

    if (err = mbedtls_ecdsa_genkey(&ecKeyPair, MBEDTLS_ECP_DP_SECP256R1, myrand, NULL))
    {
        Serial.printf("mbedtls_ecdsa_genkey = %X\r\n", err);
        throw new Error(err);
    }

    // Init pk context
    mbedtls_pk_context key;
    mbedtls_pk_init(&key);

    const mbedtls_pk_info_t *pk_info = mbedtls_pk_info_from_type(MBEDTLS_PK_ECKEY);
    if( err = mbedtls_pk_setup(&key, pk_info) ) {
        Serial.printf("mbedtls_pk_setup = %X\r\n", err);
        throw new Error(err);
    }
    key.pk_ctx = &ecKeyPair;

    unsigned char *pBuff = (unsigned char *)malloc(16000);
    memset( pBuff, 0, 16000);
    if( err = mbedtls_pk_write_pubkey_pem(&key, pBuff, 16000) ) {
        Serial.printf("mbedtls_pk_write_pubkey_pem = %X\r\n", err);
        throw new Error(err);
    }
    
    int dataLen = mbedtls_pk_write_key_der(&key, pBuff, 16000);
    Serial.printf("mbedtls_pk_write_key_der = %d\r\n", dataLen);
    unsigned char *pBuff2 = pBuff + 16000 - dataLen; 
    dumpHex( pBuff2, dataLen );

    ConfigSetBlob("deviceKey", pBuff2, dataLen);

    free(pBuff);

    mbedtls_ecdsa_free(&ecKeyPair);
}

int getPublicKeyPEM( char *buff, size_t buffLen ) {
    int err;
    mbedtls_pk_context key;
    if( err = loadKeyPair( &key ) ) {
        return err;
    }
    if( err = mbedtls_pk_write_pubkey_pem(&key, (unsigned char *)buff, buffLen) ) {
        return err;
    }
    return err;
}

int loadKeyPair( mbedtls_pk_context *key )
{
    int err = 0;
    unsigned char *pBuff = (unsigned char *)malloc(16000);
    size_t buffLen = 16000;
    memset( pBuff, 0, 16000);

    if( ConfigGetBlob("deviceKey", pBuff, &buffLen ) ) {
        free(pBuff);
        return err;
    }
    Serial.printf("ConfigGetBlob = %d\r\n", buffLen);

    mbedtls_pk_init(key);
    if( err = mbedtls_pk_parse_key(key, pBuff, buffLen, nullptr, 0) ) {
        Serial.printf("mbedtls_pk_parse_key = %X\r\n", err);
        free(pBuff);
        return err;
    }

    free(pBuff);
}

void hashData( const void *message, int dataLen, unsigned char* hashBuff) {
    int err = 0;
    mbedtls_md_context_t ctx;
    mbedtls_md_init( &ctx );
    if( err = mbedtls_md_setup(
        &ctx,
        mbedtls_md_info_from_type( MBEDTLS_MD_SHA256 ),
        0
    ) ) {
        Serial.printf("mbedtls_md_setup = %X\r\n", err);
        throw new Error(err);
    }

    if( err = mbedtls_md_starts(&ctx) ) {
        Serial.printf("mbedtls_md_starts = %X\r\n", err);
        throw new Error(err);
    }

    if( err = mbedtls_md_update(
        &ctx,
        (const unsigned char*)message,
        dataLen
    ) ) {
        Serial.printf("mbedtls_md_update = %X\r\n", err);
        throw new Error(err);
    }

    if( err = mbedtls_md_finish(
        &ctx,
        hashBuff
    ) ) {
        Serial.printf("mbedtls_md_finish = %X\r\n", err);
        throw new Error(err);
    }

    mbedtls_md_free( &ctx);
}

void hashPublicKey( mbedtls_pk_context *key, unsigned char* hashBuff) {
    
    size_t buffLen = 4096;
    unsigned char *pBuff = (unsigned char *)malloc(buffLen+1);
    memset( pBuff, 0, buffLen+1);
    int dataLen = mbedtls_pk_write_pubkey_der(key, pBuff, buffLen);
    unsigned char *pBuff2 = pBuff + buffLen - dataLen; 
    Serial.printf("Pubkey DER before hash:\r\n");
    dumpHex( pBuff2, dataLen + 1 );
    hashData( pBuff2, dataLen, hashBuff );
    free(pBuff);
}

void signHash( mbedtls_pk_context *key, const unsigned char *hash, unsigned char *pBuff, size_t *buffSize )
{
    int err = 0;

    if( err = mbedtls_pk_sign(
        key, 
        MBEDTLS_MD_SHA256,
        hash, 32,
        pBuff, buffSize,
        myrand, NULL)        
    ) {
        Serial.printf("mbedtls_pk_sign = %X\r\n", err);
        throw new Error(err);
    }

    Serial.printf("mbedtls_pk_sign -> buffSize = %d\r\n", *buffSize);
    for( int i2 =0; i2 < *buffSize; i2 ++ ) {
        Serial.printf("%02X ", pBuff[i2]);
    }
    Serial.printf("\r\n");

}

void ECCKeyScreen::onSetup()
{
}

void ECCKeyScreen::onLongPress()
{
    clear();
    int ret = 0;
    size_t sig_len;
    size_t hash_len;

    M5.Lcd.println("Generating ECC key\r\n");
    generateECKeyPair();

    unsigned char hash[32];
    char *message = "Minden cica aranyos.";
    hashData( message, strlen( message), hash);
    Serial.printf("Hash of the message: \r\n");
    for( int i2 =0; i2 < 32; i2 ++ ) {
        Serial.printf("%02X ", hash[i2]);
    }
    Serial.printf("\r\n");


    // Init pk context
    mbedtls_pk_context key;
    loadKeyPair( &key );

    unsigned char *pBuff = (unsigned char *)malloc(16000);
    ret = mbedtls_pk_write_pubkey_pem(&key, pBuff, 16000);
    M5.Lcd.printf("mbedtls_pk_write_pubkey_pem = %X\r\n", ret);
    Serial.printf("mbedtls_pk_write_pubkey_pem = %X\r\n", ret);
    Serial.printf("The generated public key: \r\n%s\r\n", pBuff);

    ret = mbedtls_pk_write_key_pem(&key, pBuff, 16000);
    M5.Lcd.printf("mbedtls_pk_write_key_pem = %X\r\n", ret);
    Serial.printf("mbedtls_pk_write_key_pem = %X\r\n", ret);
    Serial.printf("The generated private key: \r\n%s\r\n", pBuff);

    free(pBuff);
    //if( ret == MBEDTLS_ERR_)

    pBuff = (unsigned char *)malloc(16000);
    sig_len = 16000;
    memset( pBuff, 0, 16000);

    signHash( &key, hash, pBuff, &sig_len);


    free(pBuff);


    M5.Lcd.printf("mbedtls_ecdsa_free\r\n");

    int genCount;
    ConfigGetIntValue("ECCGenCount", &genCount, 0);
    ConfigSetIntValue("ECCGenCount", genCount + 1);
    onRepaint();
}

void ECCKeyScreen::onRepaint()
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

static int myrand(void *rng_state, unsigned char *output, size_t len)
{
    size_t use_len;
    int rnd;

    if (rng_state != NULL)
    {
        rng_state = NULL;
    }

    while (len > 0)
    {
        use_len = len;
        if (use_len > sizeof(int))
        {
            use_len = sizeof(int);
        }

        // TODO: use esp32_random(), and turn on wifi before
        rnd = rand();
        memcpy(output, &rnd, use_len);
        output += use_len;
        len -= use_len;
    }

    return 0;
}
