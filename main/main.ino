#include <M5StickC.h>
#include <cstring>
#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"

#include "screens.h"
#include "battery.h"
#include "ble.h"

void setup()
{
    // put your setup code here, to run once:
    printf("Setup begins\r\n");

    M5.begin();
    M5.Lcd.fillScreen(BLACK);
    M5.Axp.EnableCoulombcounter();
    M5.MPU6886.Init();

    Screens.add(new Screen(0, "Battery", 1000, &printAxp));
    Screens.add(new Screen(1, "Bluetooth", 0, &printBle));
    Screens.add(new Screen(2, "Time", 0, &printTime));
    Screens.add(new Screen(3, "Gyroscope", 200, &printGyro));

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

void printGyro(Screen *pScreen)
{
    float accX = 0;
    float accY = 0;
    float accZ = 0;

    float gyroX = 0;
    float gyroY = 0;
    float gyroZ = 0;

    float temp = 0;

    M5.MPU6886.getGyroData(&gyroX, &gyroY, &gyroZ);
    M5.MPU6886.getAccelData(&accX, &accY, &accZ);
    M5.MPU6886.getTempData(&temp);

    M5.Lcd.setCursor(0, 0, 1);
    M5.Lcd.printf("\r\n");
    M5.Lcd.printf("Gyroscope\r\n");
    M5.Lcd.printf("X : %6.2f\r\n", gyroX);
    M5.Lcd.printf("Y : %6.2f\r\n", gyroY);
    M5.Lcd.printf("Z : %6.2f\r\n", gyroZ);
    M5.Lcd.printf(" [deg/sec]\r\n");
    M5.Lcd.printf("\r\n");
    M5.Lcd.printf("Accelerometer\r\n");
    M5.Lcd.printf("X : %4.0f\r\n", accX * 1000);
    M5.Lcd.printf("Y : %4.0f\r\n", accY * 1000);
    M5.Lcd.printf("Z : %4.0f\r\n", accZ * 1000);
    M5.Lcd.printf(" [mg]\r\n");
    M5.Lcd.printf("\r\n");
}
