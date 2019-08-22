#define ESP32
#include <M5StickC.h>
#include <cstring>
#include "screens.h"

void Screen::onEnter()
{
    M5.Lcd.fillScreen(BLACK);
    onRefresh();
}

void Screen::onLongPress()
{
    M5.Lcd.println("LONG PRESS");
}

void Screen::onRefresh()
{
    if (screenProc != NULL)
    {
        (screenProc)(this);
    }
};

ScreenMgr Screens;

void ScreenMgr::add(Screen *screen)
{
    Screen **tmpArray = new Screen *[screenArraySize + 1];
    for (int i = 0; i < screenArraySize; i++)
    {
        tmpArray[i] = screenArray[i];
    }
    tmpArray[screenArraySize] = screen;
    delete screenArray;
    screenArray = tmpArray;
    screenArraySize++;
}

Screen *ScreenMgr::currentScreen()
{
    return screenArray[currentScreenIdx];
}

void ScreenMgr::setupScreens(){
    for( int i = 0; i < screenArraySize; i++) {
        screenArray[i]->onSetup();
    }
}

void ScreenMgr::processUIActions()
{
    unsigned long now = millis();
    uint8_t btnAPrevState = M5.BtnA.isPressed();
    uint8_t btnAThisState = M5.BtnA.read();
    if (btnAPrevState == 0 && btnAThisState > 0)
    {
        longpressStartred = now;
    }
    else if (btnAPrevState > 0 && btnAThisState == 0 && longpressShouldClear)
    {
        longpressShouldClear = false;
    }
    else if (btnAPrevState > 0 && btnAThisState == 0 && !longpressShouldClear)
    {
        // Button A released
        longpressStartred = now;
        currentScreen()->onLeave();
        currentScreenIdx = currentScreenIdx + 1;
        if (currentScreenIdx == screenArraySize)
        {
            currentScreenIdx = 0;
        }
        currentScreen()->onEnter();

        currentScreen()->lastPrinted = now;
        M5.Lcd.fillRect(0, 140, 80, 20, TFT_NAVY);
        uint32_t savedTextColor = M5.Lcd.textcolor;
        uint32_t savedBgColor = M5.Lcd.textbgcolor;
        uint8_t savedTextFont = M5.Lcd.textfont;
        uint8_t savedTextSize = M5.Lcd.textsize;

        int font = 2;
        M5.Lcd.setTextColor(YELLOW, TFT_NAVY);
        M5.Lcd.setTextFont(font);
        M5.Lcd.setTextSize(1);
        int width = M5.Lcd.textWidth(currentScreen()->name, font);
        int heigth = M5.Lcd.fontHeight(font);
        int x = (M5.Lcd.width() - width) / 2;
        int y = ((20 - heigth) / 2) + 140;
        M5.Lcd.drawString(currentScreen()->name, x, y);

        M5.Lcd.setTextColor(savedTextColor, savedBgColor);
        M5.Lcd.setTextFont(savedTextFont);
        M5.Lcd.setTextSize(savedTextSize);
    }
    else if (btnAPrevState > 0 && btnAThisState > 0)
    {
        // Wait for longpress
        if ((now - longpressStartred) > longpressDuration)
        {
            currentScreen()->onLongPress();
            longpressStartred = now;
            longpressShouldClear = true;
        }
    }

    // Scheck screen refresh
    if (currentScreen()->refreshPeriod > 0)
    {
        if ((now - currentScreen()->lastPrinted) > currentScreen()->refreshPeriod)
        {
            currentScreen()->onRefresh();
            currentScreen()->lastPrinted = now;
        }
    }
}
