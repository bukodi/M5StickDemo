#include "screens.h"

class ECCKeyScreen : public Screen
    
{
    public:
    ECCKeyScreen() : Screen("ECC Key", 1000, NULL) {

    };
    void onSetup();
    void onRefresh();
    void onLongPress();
};

