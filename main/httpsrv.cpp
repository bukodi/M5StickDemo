#include <ESPAsyncWebServer.h>

#include "httpsrv.h"

// Set web server port number to 80
AsyncWebServer *pWebServer;

void handleIndexGET(AsyncWebServerRequest *request)
{
    const char *html =
        "<!DOCTYPE html>"
        "<html>"
        "<body>"
        "  <h2>M5 Sctick Demo</h2><br>"
        "  <a href=\"/wifi\">WiFi setup</a><br>"
        "  <a href=\"/pubkey\">Public key</a><br>"
        "</body>"
        "</html>";
    request->send(200, "text/html", html);
}

void handleWifiGET(AsyncWebServerRequest *request)
{
    const char *html =
        "<!DOCTYPE html>"
        "<html>"
        "<body>"
        "  <h2>WiFi setup</h2>"
        "  <form action=\"/setup\">"
        "    SSID:<br>"
        "    <input type=\"text\" name=\"SSID\">"
        "    <br>"
        "    Password:<br>"
        "    <input type=\"text\" name=\"password\">"
        "    <br><br>"
        "    <input type=\"submit\" value=\"Add\">"
        "  </form>"
        "</body>"
        "</html>";
    request->send(200, "text/html", html);
}

bool HttpSrv_isStarted()
{
    return pWebServer != nullptr;
}

void HttpSrv_Start()
{
    pWebServer = new AsyncWebServer(80);
    pWebServer->on("/", HTTP_GET, handleIndexGET);
    pWebServer->on("/wifi", HTTP_GET, handleWifiGET);
    pWebServer->begin();
}

void HttpSrv_Stop()
{
    pWebServer->end();
    pWebServer->~AsyncWebServer();
    pWebServer = nullptr;
}
