#ifndef _SCREENS_H /* include guards */
#define _SCREENS_H

#include <Arduino.h>

class Screen
{
public:
    char *name;
    void (*screenProc)(Screen *pScreen);
    unsigned long timerPeriod;
    unsigned long lastTimerTick;

    Screen(char *pname, unsigned long p_timerPeriod, void (*pscreenProc)(Screen *pScreen))
    {
        name = pname;
        screenProc = pscreenProc;
        timerPeriod = p_timerPeriod;
    }

    virtual void clear();

    virtual void onSetup(){};
    virtual void onEnter();
    virtual void onLeave(){};
    virtual void onRepaint();
    virtual void onTimerTick(){
        onRepaint();
    };
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