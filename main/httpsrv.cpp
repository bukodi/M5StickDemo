#include <ESPAsyncWebServer.h>

#include "ecckey.h"
#include "httpsrv.h"
#include "settings.h"
#include "wifi.h"

// Set web server port number to 80
AsyncWebServer *pWebServer;

const String head =
    "<!DOCTYPE html>"
    "<html>"
    "<head>"
    "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"/>"
    "</head>"
    "<body>";
const String tail =
    "</body>"
    "</html>";

void handleIndexGET(AsyncWebServerRequest *request)
{
    String body =
        "  <h2>M5 Sctick Demo</h2><br>"
        "  <a href=\"/wifi\">WiFi setup</a><br>"
        "  <a href=\"/pubkey\">Public key</a><br>";
    request->send(200, "text/html", head + body + tail);
}

void handleWifiGET(AsyncWebServerRequest *request)
{
    String body =
        "  <h2>WiFi setup</h2>"
        "  <form action=\"/wifi\" method=\"post\">"
        "    SSID:<br>"
        "    <input type=\"text\" name=\"SSID\">"
        "    <br>"
        "    Password:<br>"
        "    <input type=\"text\" name=\"password\">"
        "    <br><br>"
        "    <input type=\"submit\" value=\"Add\">"
        "  </form>";
    request->send(200, "text/html", head + body + tail);
}

void handleWifiPOST(AsyncWebServerRequest *request)
{
    if (request->hasParam("SSID", true))
    {
        AsyncWebParameter *p = request->getParam("SSID", true);
        ConfigSetStrValue(ConfigKey_WiFiSTA_SSID, p->value().c_str());
    }
    if (request->hasParam("password", true))
    {
        AsyncWebParameter *p = request->getParam("password", true);
        ConfigSetStrValue(ConfigKey_WiFiSTA_Password, p->value().c_str());
    }
    String body =
        "  <h2>WiFi setting saved.</h2><br>"
        "  <a href=\"/\">Index</a><br>";
    request->send(200, "text/html", head + body + tail);
}

void handlePubKeyGET(AsyncWebServerRequest *request)
{
    char *buff = (char *)malloc(4096);
    getPublicKeyPEM( buff, 4096);
    String body =
        "  <h2>Public KEY</h2>"
        "  <pre>";
    body += buff;
    body += "  </pre>";
    request->send(200, "text/html", head + body + tail);
}

bool HttpSrv_isStarted()
{
    return pWebServer != nullptr;
}

void HttpSrv_Start()
{
    if (pWebServer != nullptr)
        return;
    pWebServer = new AsyncWebServer(80);
    pWebServer->on("/", HTTP_GET, handleIndexGET);
    pWebServer->on("/wifi", HTTP_GET, handleWifiGET);
    pWebServer->on("/wifi", HTTP_POST, handleWifiPOST);
    pWebServer->on("/pubkey", HTTP_GET, handlePubKeyGET);
    pWebServer->begin();
}

void HttpSrv_Stop()
{
    if (pWebServer == nullptr)
        return;
    pWebServer->end();
    pWebServer->~AsyncWebServer();
    pWebServer = nullptr;
}
