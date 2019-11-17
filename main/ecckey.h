#include "screens.h"

class ECCKeyScreen : public Screen
    
{
    public:
    ECCKeyScreen() : Screen("ECC Key", 0, NULL) {

    };
    void onSetup();
    void onRepaint();
    void onLongPress();
};

