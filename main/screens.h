#ifndef _SCREENS_H /* include guards */
#define _SCREENS_H

#include <Arduino.h>

class Screen
{
public:
    char *name;
    unsigned long timerPeriod = 0;
    unsigned long timerLastTick;
    bool timerActiveInBackground;


    Screen(char *pname)
    {
        name = pname;
    }

    void startUITimer(unsigned long periodInMs, bool activeInBackground = false) {
        timerPeriod = periodInMs;
        timerActiveInBackground = activeInBackground;
    }

    void stopUITimer() {
        timerPeriod = 0;
    }

    void activate();

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
    int nextScreenIdx;
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
    void checkScreenTimers();
    void activate(Screen *screen);
};

extern ScreenMgr Screens;

#endif /* _SCREENS_H */