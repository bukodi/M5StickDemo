#include "screens.h"

class BleScreen : public Screen
    
{
    public:
    BleScreen() : Screen(1, "Bluetooth", 1000, NULL) {

    };
    void onSetup();
    void onRefresh();
    void onLongPress();
};

