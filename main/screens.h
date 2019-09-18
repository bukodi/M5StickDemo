#ifndef _SCREENS_H /* include guards */
#define _SCREENS_H

#include <Arduino.h>

class Screen
{
public:
    char *name;
    void (*screenProc)(Screen *pScreen);
    unsigned long refreshPeriod;
    unsigned long lastPrinted;

    Screen(char *pname, unsigned long prefreshPeriod, void (*pscreenProc)(Screen *pScreen))
    {
        name = pname;
        screenProc = pscreenProc;
        refreshPeriod = prefreshPeriod;
    }

    virtual void onSetup(){};
    virtual void onEnter();
    virtual void onLeave(){};
    virtual void onRefresh();
    virtual void onLongPress();
};

class ScreenMgr
{
private:
    Screen **screenArray;
    int screenArraySize;
    int currentScreenIdx;
    unsigned long longpressStartred;
    unsigned long longpressDuration;
    bool longpressShouldClear;

public:
    ScreenMgr()
    {
        screenArray = nullptr;
        screenArraySize = 0;
        currentScreenIdx = 0;
        longpressDuration = 1000;
        longpressStartred = false;
        longpressShouldClear = false;
    };

    void setupScreens();
    void add(Screen *screen);
    void processUIActions();
    Screen *currentScreen();
};

extern ScreenMgr Screens;

#endif /* _SCREENS_H */