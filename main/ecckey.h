#include "screens.h"

class ECCKeyScreen : public Screen
    
{
    public:
    ECCKeyScreen() : Screen("ECC Key") {

    };
    void onSetup();
    void onRepaint();
    void onLongPress();
};


