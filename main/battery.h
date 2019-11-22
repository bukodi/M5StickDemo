#include "screens.h"

class BatteryScreen : public Screen
    
{
    public:
    BatteryScreen() : Screen("Battery") {
    };
    void onSetup();
    void onRepaint();
    void onLongPress();
};

