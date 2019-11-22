#include <M5StickC.h>
#include <WiFi.h>
#include <WiFiAP.h>
#include <nvs.h>

#include "wifi.h"
#include "httpsrv.h"
#include "settings.h"

extern "C"
{
#include <esp_err.h>
#include <esp_wifi.h>
#include <esp_event_loop.h>
}

void WifiSetupScreen::onSetup()
{
}

void WifiSetupScreen::onLongPress()
{
    if (WiFi.getMode() & WIFI_MODE_AP)
    {
        M5.Lcd.printf("\r\nStop WiFi\r\n");
        HttpSrv_Stop();
        WiFi.softAPdisconnect(true);
        WiFi.disconnect();
        onRepaint();
    }
    else
    {
        M5.Lcd.printf("\r\nStart WiFi\r\n");
        WiFi.disconnect();

        byte mac[6];
        WiFi.macAddress(mac);

        char ssid[30];
        sprintf(ssid, "M5Stick-%02X%02X", mac[4], mac[5]);

        bool isOk = WiFi.mode(WIFI_MODE_AP);
        isOk = WiFi.softAP(ssid, nullptr);
        HttpSrv_Start();
        onRepaint();
    }
}

void WifiSetupScreen::onTimerTick()
{
}

void WifiSetupScreen::onRepaint()
{
    clear();
    if (WiFi.getMode() & WIFI_MODE_AP)
    {
        M5.Lcd.printf("\r\n");
        M5.Lcd.printf("WiFi active\r\n");
        M5.Lcd.printf("\r\n");

        wifi_config_t conf_current;
        esp_wifi_get_config(WIFI_IF_AP, &conf_current);
        char *ssid = (char *)conf_current.ap.ssid;
        M5.Lcd.printf("AP SSID:\r\n%s\r\n", ssid);
        M5.Lcd.printf((char *)conf_current.ap.password);
        M5.Lcd.printf("\r\n");
        M5.Lcd.printf("IP address:\r\n%s\r\n", WiFi.softAPIP().toString().c_str());
        M5.Lcd.printf("\r\n");

        M5.Lcd.printf("\r\nLong press:\r\n");
        M5.Lcd.printf("Stop\r\n");
    }
    else
    {
        M5.Lcd.printf("\r\nWiFi inactive\r\n");

        M5.Lcd.printf("\r\nLong press:\r\n");
        M5.Lcd.printf("Start\r\n");
    }
}

bool isSTAConfigured()
{
    char ssid[64];
    int err = ConfigGetStrValue(ConfigKey_WiFiSTA_SSID, ssid, sizeof(ssid));
    if (err == ESP_ERR_NVS_NOT_FOUND)
    {
        return false;
    }
    return true;
}

void WifiClientScreen::startSTA()
{
    char ssid[32];
    char password[64];
    ConfigGetStrValue(ConfigKey_WiFiSTA_SSID, ssid, sizeof(ssid));
    ConfigGetStrValue(ConfigKey_WiFiSTA_Password, password, sizeof(password));

    int err = WiFi.begin(ssid, password);
    M5.Lcd.print("Connecting");
    startUITimer( 500 );
}

void WifiClientScreen::onTimerTick()
{
    Serial.printf("Wifi status : %d\r\n", WiFi.status());
    if( WiFi.status() == WL_CONNECTED ) {
        M5.Lcd.printf("\r\nConnected\r\n");
        stopUITimer();
    } else {
        M5.Lcd.print(".");
    }
}

void WifiClientScreen::onSetup()
{
    int autoconnect;
    ConfigGetIntValue(ConfigKey_WiFiSTA_autoconnect, &autoconnect, 0);

    if (autoconnect && isSTAConfigured() )
    {
        startSTA();
    }

}

void WifiClientScreen::onLongPress()
{
    if (WiFi.isConnected())
    {
        M5.Lcd.printf("\r\nStop WiFi\r\n");
        HttpSrv_Stop();
        WiFi.disconnect();
        onRepaint();
    }
    else
    {
        M5.Lcd.printf("\r\nStart WiFi\r\n");
        startSTA();
        HttpSrv_Start();
        onRepaint();
    }
}

void WifiClientScreen::onRepaint()
{
    clear();

    if (!isSTAConfigured())
    {
        M5.Lcd.printf("\r\n");
        M5.Lcd.printf("WiFi isn't\r\nconfigured\r\n");
        M5.Lcd.printf("\r\n");
    }
    else if (WiFi.isConnected())
    {
        M5.Lcd.printf("\r\n");
        M5.Lcd.printf("WiFi active\r\n");
        M5.Lcd.printf("\r\n");

        wifi_config_t conf_current;
        esp_wifi_get_config(WIFI_IF_STA, &conf_current);
        M5.Lcd.printf("STA SSID:\r\n%s\r\n", conf_current.sta.ssid);
        M5.Lcd.printf("Channel:\r\n%d\r\n", conf_current.sta.channel);
        M5.Lcd.printf("\r\n");
        M5.Lcd.printf("IP address:\r\n%s\r\n", WiFi.localIP().toString().c_str());
        M5.Lcd.printf("\r\n");

        M5.Lcd.printf("\r\nLong press:\r\n");
        M5.Lcd.printf("Stop\r\n");
    }
    else
    {
        M5.Lcd.printf("\r\nWiFi inactive\r\n");

        char ssid[64];
        ConfigGetStrValue(ConfigKey_WiFiSTA_SSID, ssid, sizeof(ssid));

        M5.Lcd.printf("STA SSID:\r\n%s\r\n", ssid);

        M5.Lcd.printf("\r\nLong press:\r\n");
        M5.Lcd.printf("Start\r\n");
    }
}
