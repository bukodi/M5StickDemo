#include <M5StickC.h>
#include <WiFi.h>
#include <WiFiAP.h>
#include <ESPAsyncWebServer.h>

#include "wifi.h"

extern "C"
{
#include <esp_err.h>
#include <esp_wifi.h>
#include <esp_event_loop.h>
}

// Replace with your network credentials
const char *ssid = "Cica-AP";
const char *password = nullptr; //"12345678";

// Set web server port number to 80
AsyncWebServer *pWebServer;

// Variable to store the HTTP request
String header;

void WifiSetupScreen::onSetup()
{
}

void WifiSetupScreen::onLongPress()
{
    if (pWebServer != nullptr)
    {
        M5.Lcd.printf("\r\nStop WiFi\r\n");
        pWebServer->end();
        pWebServer->~AsyncWebServer();
        pWebServer = nullptr;
        WiFi.softAPdisconnect(true);
        onRepaint();
    }
    else
    {
        M5.Lcd.printf("\r\nStart WiFi\r\n");
        WiFi.disconnect();
        bool isOk = WiFi.mode(WIFI_AP_STA);
        isOk = WiFi.softAP(ssid, password);

        pWebServer = new AsyncWebServer(80);

        pWebServer->on("/hello", HTTP_GET, [](AsyncWebServerRequest *request){
            request->send(200, "text/plain", "Hello World");
        });
        pWebServer->begin();
        onRepaint();
    }
}

void WifiSetupScreen::onTimerTick()
{
}

void WifiSetupScreen::onRepaint()
{
    clear();
    if (pWebServer != nullptr )
    {
        M5.Lcd.printf("\r\nWiFi active\r\n");
        M5.Lcd.printf(WiFi.softAPIP().toString().c_str());
        M5.Lcd.printf("\r\n");

        wifi_config_t conf_current;
        esp_wifi_get_config(WIFI_IF_AP, &conf_current);
        char *ssid = (char *)conf_current.ap.ssid;
        M5.Lcd.printf(ssid);
        M5.Lcd.printf("\r\n");
        M5.Lcd.printf((char *)conf_current.ap.password);
        M5.Lcd.printf("\r\n");
        M5.Lcd.printf(WiFi.softAPIP().toString().c_str());
        M5.Lcd.printf("\r\n");
        M5.Lcd.printf("Long press:\r\n");
        M5.Lcd.printf("Stop\r\n");
    }
    else
    {
        M5.Lcd.printf("\r\nWiFi inactive\r\n");
        M5.Lcd.printf("\r\n");
        M5.Lcd.printf("Long press:\r\n");
        M5.Lcd.printf("Start\r\n");
    }
}
