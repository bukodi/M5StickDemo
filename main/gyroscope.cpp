#include <M5StickC.h>
#include "gyroscope.h"

void GyroscopeScreen::onSetup()
{
    M5.MPU6886.Init();
}

void GyroscopeScreen::onLongPress()
{
}

void GyroscopeScreen::onRefresh()
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
