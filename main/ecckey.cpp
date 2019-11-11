#include <M5StickC.h>
#include "ecckey.h"
#include "mbedtls/x509_crt.h"
#include "mbedtls/ecp.h"
#include "mbedtls/ecdsa.h"
#include "mbedtls/pk.h"

mbedtls_ecdsa_context ecKeyPair;

static int myrand(void *rng_state, unsigned char *output, size_t len)
{
    size_t use_len;
    int rnd;

    if (rng_state != NULL) {
        rng_state  = NULL;
    }

    while (len > 0) {
        use_len = len;
        if (use_len > sizeof(int)) {
            use_len = sizeof(int);
        }

        rnd = rand();
        memcpy(output, &rnd, use_len);
        output += use_len;
        len -= use_len;
    }

    return 0;
}

void ECCKeyScreen::onSetup()
{
}

void ECCKeyScreen::onLongPress()
{
    int ret = 0;
    size_t sig_len;
    size_t hash_len;
    
    M5.Lcd.println("Generating ECC key\r\n");
    mbedtls_ecdsa_init(&ecKeyPair);
    ret = mbedtls_ecdsa_genkey(&ecKeyPair, MBEDTLS_ECP_DP_SECP256K1, myrand, NULL);
    M5.Lcd.printf("mbedtls_ecdsa_genkey = %X\r\n", ret );    
    Serial.printf("mbedtls_ecdsa_genkey = %X\r\n", ret );
    
    // Init pk context
    mbedtls_pk_context key;
    mbedtls_pk_init( &key );
    const mbedtls_pk_info_t *pk_info = mbedtls_pk_info_from_type(MBEDTLS_PK_ECKEY);
    ret = mbedtls_pk_setup( &key, pk_info );
    M5.Lcd.printf("mbedtls_pk_setup = %X\r\n", ret );    
    Serial.printf("mbedtls_pk_setup = %X\r\n", ret );
    key.pk_ctx = &ecKeyPair;

    unsigned char *pBuff = (unsigned char *)malloc( 16000 );
    ret = mbedtls_pk_write_pubkey_pem(&key, pBuff, 16000 );
    M5.Lcd.printf("mbedtls_pk_write_pubkey_pem = %X\r\n", ret );    
    Serial.printf("mbedtls_pk_write_pubkey_pem = %X\r\n", ret );

    Serial.printf("The generated public key: \r\n%s\r\n", pBuff );

    free( pBuff );
    //if( ret == MBEDTLS_ERR_)

    mbedtls_ecdsa_free(&ecKeyPair);
    M5.Lcd.printf("mbedtls_ecdsa_free\r\n");

}

void ECCKeyScreen::onRefresh()
{

    M5.Lcd.setCursor(0, 0, 1);
    M5.Lcd.printf("\r\n");
    M5.Lcd.printf("ECCKey\r\n");
    M5.Lcd.printf("\r\n");
}

