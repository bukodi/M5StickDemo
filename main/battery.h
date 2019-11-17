#include "screens.h"

class BatteryScreen : public Screen
    
{
    public:
    BatteryScreen() : Screen("Battery", 100, NULL) {

    };
    void onSetup();
    void onRepaint();
    void onLongPress();
};

