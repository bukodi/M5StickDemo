#include <M5StickC.h>
#include "nvs_flash.h"
#include "nvs.h"

#include "settings.h"

#define CONFIG_NAMESPACE "demo"

bool initCompleted = false;

void init()
{
    if (initCompleted)
    {
        return;
    }
    int err = nvs_flash_init();
    Serial.printf("nvs_flash_init = %X\r\n", err);
    initCompleted = true;
}

int ConfigGetIntValue( char *key, int *retValue, int defaultValue ) {
    init();
    nvs_handle nvsHandle = 0L;
    int err = nvs_open(CONFIG_NAMESPACE, NVS_READWRITE, &nvsHandle);
    if( err ) {
        return err;
    }

    err = nvs_get_i32(nvsHandle, key, retValue);
    switch (err) {
        case ESP_OK:                
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            err = nvs_set_i32(nvsHandle, key, defaultValue);
            if( err ) {
                break;
            }
            *retValue = defaultValue;
            err = nvs_commit(nvsHandle);
            break;
        default:
            break;

    }
    nvs_close(nvsHandle);                   
    return err;
}

int ConfigGetStrValue( char *key, char *retBuff, int buffLen ) {
    init();
    nvs_handle nvsHandle = 0L;
    int err = nvs_open(CONFIG_NAMESPACE, NVS_READWRITE, &nvsHandle);
    if( err ) {
        return err;
    }

    size_t buffLen2 = buffLen;
    err = nvs_get_str(nvsHandle, key, retBuff, &buffLen2);
    switch (err) {
        case ESP_OK:                
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            break;
        default:
            break;
    }
    nvs_close(nvsHandle);                   
    return err;
}

int ConfigSetIntValue( char *key, int value ) {
    init();
    nvs_handle nvsHandle = 0L;
    int err = nvs_open(CONFIG_NAMESPACE, NVS_READWRITE, &nvsHandle);
    if( err ) {
        return err;
    }

    err = nvs_set_i32(nvsHandle, key, value);
    if( !err ) {
        err = nvs_commit(nvsHandle);
    } 

    nvs_close(nvsHandle);                   
    return err;
}

int ConfigSetStrValue( char *key, char* value ) {
    init();
    nvs_handle nvsHandle = 0L;
    int err = nvs_open(CONFIG_NAMESPACE, NVS_READWRITE, &nvsHandle);
    if( err ) {
        return err;
    }

    err = nvs_set_str(nvsHandle, key, value);
    if( !err ) {
        err = nvs_commit(nvsHandle);
    } 

    nvs_close(nvsHandle);                   
    return err;
}

void SettingsScreen::onSetup()
{
    init();
}

void SettingsScreen::onLongPress()
{
}

void SettingsScreen::onRepaint()
{
}
