#include "screens.h"

class BleScreen : public Screen
    
{
    public:
    BleScreen() : Screen(1, "Bluetooth", 0, NULL) {

    };
    void onRefresh();
    void onLongPress();
};

void printBle(Screen *pScreen);
