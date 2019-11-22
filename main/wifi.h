#ifndef _WIFI_H /* include guards */
#define _WIFI_H

#include "screens.h"

const char *const ConfigKey_WiFiSTA_SSID = "STA.SSID";
const char *const ConfigKey_WiFiSTA_Password = "STA.passw";
const char *const ConfigKey_WiFiSTA_autoconnect = "STA.autoconn";

class WifiSetupScreen : public Screen
{
public:
    WifiSetupScreen() : Screen("WiFi Setup"){

                        };
    void onSetup();
    void onRepaint();
    void onLongPress();
    void onTimerTick();
};

class WifiClientScreen : public Screen
{
public:
    WifiClientScreen() : Screen("WiFi Client"){

                         };
    void onSetup();
    void onRepaint();
    void onLongPress();
    void onTimerTick();

    void startSTA();
};

#endif /* _WIFI_H */