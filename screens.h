#include <Arduino.h>

class Screen
{
public:
    int id;
    char *name;
    void (*screenProc)(Screen *pScreen);
    unsigned long refreshPeriod;
    unsigned long lastPrinted;

    Screen(int pid, char *pname, unsigned long prefreshPeriod, void (*pscreenProc)(Screen *pScreen))
    {
        id = pid;
        name = pname;
        screenProc = pscreenProc;
        refreshPeriod = prefreshPeriod;
    }

    void repaint();
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

    void add(Screen *screen);
    void processUIActions();
    Screen *currentScreen();
};              

extern ScreenMgr Screens;
