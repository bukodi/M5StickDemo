#ifndef _DEEP_SLEEP_H /* include guards */
#define _DEEP_SLEEP_H

#include "screens.h"

class DeepSleepScreen : public Screen
{
public:
    DeepSleepScreen() : Screen("Deep Sleep"){

                         };
    void onSetup();
    void onRepaint();
    void onLongPress();
    void onTimerTick();

    void startSleep();
};

#endif /* _DEEP_SLEEP_H */