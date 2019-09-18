#include "screens.h"

class BleScreen : public Screen
    
{
    public:
    BleScreen() : Screen("Bluetooth", 1000, NULL) {

    };
    void onSetup();
    void onRefresh();
    void onLongPress();
};

