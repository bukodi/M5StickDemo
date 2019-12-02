#define ESP32
#include <M5StickC.h>
#include <cstring>
#include "screens.h"

#define MENU_HEIGHT 20

void Screen::clear()
{
    M5.Lcd.fillRect(0, 0, M5.Lcd.width(), M5.Lcd.height() - MENU_HEIGHT, BLACK);
    M5.Lcd.setCursor(0, 0, 1);
}

void Screen::onEnter()
{
    M5.Lcd.fillScreen(BLACK);
    onRepaint();
}

void Screen::onLongPress()
{
    M5.Lcd.println("LONG PRESS");
}

void Screen::onRepaint(){};

void Screen::activate() {
    Screens.activate( this );
}


ScreenMgr Screens;

void ScreenMgr::activate(Screen *screen) {
    for (int i = 0; i < screenArraySize; i++)
    {
        if( screenArray[i] == screen ) {
            nextScreenIdx = i;
        };
    }
}

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

void ScreenMgr::setupScreens()
{
    for (int i = 0; i < screenArraySize; i++)
    {
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
        nextScreenIdx = currentScreenIdx + 1;
        if (nextScreenIdx == screenArraySize)
        {
            nextScreenIdx = 0;
        }
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

    // Change screen
    if( currentScreenIdx != nextScreenIdx ) {
        currentScreen()->onLeave();
        currentScreenIdx = nextScreenIdx;
        currentScreen()->onEnter();

        currentScreen()->timerLastTick = now;
        M5.Lcd.fillRect(0, M5.Lcd.height() - MENU_HEIGHT, M5.Lcd.width(), MENU_HEIGHT, TFT_NAVY);
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

    // Check screen timers
    for (int i = 0; i < screenArraySize; i++)
    {
        if( screenArray[i]->timerPeriod > 0 ) {
            if( screenArray[i]->timerActiveInBackground || i == currentScreenIdx ) {
                if ((now - screenArray[i]->timerLastTick) > screenArray[i]->timerPeriod)
                {
                    screenArray[i]->onTimerTick();
                    screenArray[i]->timerLastTick = now;
                }                
            }
        };
    }
}
