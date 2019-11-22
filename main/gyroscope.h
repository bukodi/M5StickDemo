#include "screens.h"

class GyroscopeScreen : public Screen
    
{
    public:
    GyroscopeScreen() : Screen("Gyroscope") {

    };
    void onSetup();
    void onRepaint();
    void onLongPress();
};

