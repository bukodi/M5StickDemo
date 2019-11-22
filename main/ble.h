#include "screens.h"

class BleScreen : public Screen
    
{
    public:
    BleScreen() : Screen("Bluetooth") {

    };
    void onSetup();
    void onRepaint();
    void onLongPress();
};

