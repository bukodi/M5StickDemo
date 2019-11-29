#include <M5StickC.h>
#include <cstring>
#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"

#include "screens.h"
#include "battery.h"
#include "gyroscope.h"
#include "ecckey.h"
#include "eccsign.h"
//#include "ble.h"
#include "wifi.h"
#include "settings.h"

void setup()
{
    Serial.begin(115200);

    printf("Setup begins\r\n");

    // Turn off led
    pinMode (10, OUTPUT);
    digitalWrite (10, HIGH);	

    M5.begin();
    M5.Lcd.fillScreen(BLACK);

    Screens.add(new ECCSignScreen());
    Screens.add(new ECCKeyScreen());
    Screens.add(new WifiSetupScreen());
    Screens.add(new WifiClientScreen());
    Screens.add(new BatteryScreen());
    //Screens.add(new SettingsScreen());
    Screens.add(new GyroscopeScreen());
    //Screens.add(new Screen("WiFi Client", 0, &printTime));
    //Screens.add(new BleScreen());

    Screens.setupScreens();
    printf("Setup completed\r\n");

    M5.Lcd.fillScreen(BLACK);
    Screens.currentScreen()->onEnter();
}

/** This is the main loop */
void loop()
{
    Screens.processUIActions();
    vTaskDelay(20);
}

void printSetup(Screen *pScreen)
{
}


