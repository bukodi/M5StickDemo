#include "screens.h"

int ConfigGetIntValue( char *key, int *retValue, int defaultValue );
int ConfigSetIntValue( char *key, int value );
int ConfigGetStrValue( char *key, char *retBuff, int buffLen );
int ConfigSetStrValue( char *key, char* value );

class SettingsScreen : public Screen    
{
    public:
    SettingsScreen() : Screen("Settings", 0, NULL) {

    };
    void onSetup();
    void onRepaint();
    void onLongPress();
};

