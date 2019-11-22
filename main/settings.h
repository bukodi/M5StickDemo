#include "screens.h"

int ConfigGetIntValue( const char *key, int *retValue, int defaultValue );
int ConfigSetIntValue( const char *key, int value );
int ConfigGetStrValue( const char *key, char *retBuff, int buffLen );
int ConfigSetStrValue( const char *key, const char* value );

class SettingsScreen : public Screen    
{
    public:
    SettingsScreen() : Screen("Settings") {

    };
    void onSetup();
    void onRepaint();
    void onLongPress();
};

