#include "screens.h"

class GyroscopeScreen : public Screen
    
{
    public:
    GyroscopeScreen() : Screen("Gyroscope", 100, NULL) {

    };
    void onSetup();
    void onRefresh();
    void onLongPress();
};

