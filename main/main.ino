#include <M5StickC.h>
#include <cstring>
#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"

#include "screens.h"
#include "battery.h"
#include "gyroscope.h"
#include "ecckey.h"
//#include "ble.h"
//#include "wifi.h"

void setup()
{
    Serial.begin(115200);

    printf("Setup begins\r\n");

    M5.begin();
    M5.Lcd.fillScreen(BLACK);

    Screens.add(new BatteryScreen());
    Screens.add(new Screen("Time", 0, &printTime));
    Screens.add(new GyroscopeScreen());
    Screens.add(new ECCKeyScreen());
    //Screens.add(new WifiSetupScreen());
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

void printTime(Screen *pScreen)
{
    M5.Lcd.setCursor(0, 0, 1);
    M5.Lcd.printf("\r\nPlace for\r\ntime settings\r\n");
}

