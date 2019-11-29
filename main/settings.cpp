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
    initCompleted = true;
}

int ConfigGetIntValue( const char *key, int *retValue, int defaultValue ) {
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

int ConfigGetStrValue( const char *key, char *retBuff, int buffLen ) {
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

int ConfigGetBlob( const char *key, void *retBuff, size_t *buffLen ) {
    init();
    nvs_handle nvsHandle = 0L;
    int err = nvs_open(CONFIG_NAMESPACE, NVS_READWRITE, &nvsHandle);
    if( err ) {
        return err;
    }

    err = nvs_get_blob(nvsHandle, key, retBuff, buffLen);
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

int ConfigSetIntValue( const char *key, int value ) {
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

int ConfigSetStrValue( const char *key, const char* value ) {
    init();
    nvs_handle nvsHandle = 0L;
    int err = nvs_open(CONFIG_NAMESPACE, NVS_READWRITE, &nvsHandle);
    if( err ) {
        throw err;
    }

    err = nvs_set_str(nvsHandle, key, value);
    if( err ) {
        throw err;
    }
    if( !err ) {
        err = nvs_commit(nvsHandle);
    } 

    nvs_close(nvsHandle);                   
    return err;
}

int ConfigSetBlob( const char *key, const void* buff, size_t buffLen ) {
    init();
    nvs_handle nvsHandle = 0L;
    int err = nvs_open(CONFIG_NAMESPACE, NVS_READWRITE, &nvsHandle);
    if( err ) {
        throw err;
    }

    err = nvs_set_blob(nvsHandle, key, buff, buffLen);
    if( err ) {
        throw err;
    }
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
    clear();
    
}
