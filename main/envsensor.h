#include "screens.h"

class EnvSensorScreen : public Screen
{
    public:
    EnvSensorScreen() : Screen("Env Sensor") {

    };
    void onSetup();
    void onRepaint();
    void onLongPress();
};

