#include "screens.h"

class WifiSetupScreen : public Screen
    
{
    public:
    WifiSetupScreen() : Screen("WiFi Setup", 100, NULL) {

    };
    void onSetup();
    void onRefresh();
    void onLongPress();
};

