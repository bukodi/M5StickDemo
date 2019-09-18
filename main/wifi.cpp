#include <M5StickC.h>
#include <WiFi.h>
#include <WiFiAP.h>
#include "wifi.h"

extern "C" {
#include <esp_err.h>
#include <esp_wifi.h>
#include <esp_event_loop.h>
}

// Replace with your network credentials
const char *ssid = "Cica-AP";
const char *password = nullptr; //"12345678";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

void WifiSetupScreen::onSetup()
{
}

void WifiSetupScreen::onLongPress()
{
    if (server)
    {
        M5.Lcd.printf("\r\nStop WiFi\r\n");
        server.end();
        //WiFi.stopSmartConfig();
        WiFi.softAPdisconnect(true);
    }
    else
    {
        M5.Lcd.printf("\r\nStart WiFi\r\n");
        WiFi.disconnect();
        bool isOk = WiFi.mode(WIFI_AP_STA);
        Serial.printf( "ssid=\"%s\"\r\n", ssid);
        Serial.println( isOk ? "Mode set OK" : "Mode set failed");
        isOk = WiFi.softAP(ssid, password);
        Serial.println( isOk ? "Start AP OK" : "Start AP failed");
        //WiFi.beginSmartConfig();
        server.begin();
    }
}

void WifiSetupScreen::onRefresh()
{
    M5.Lcd.setCursor(0, 0, 1);
    if (server)
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

    }
    else
    {
        M5.Lcd.printf("\r\nWiFi inactive\r\n");
    }
}
