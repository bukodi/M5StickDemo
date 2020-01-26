#include "deepsleep.h"
#include <driver/rtc_io.h>
#include <M5StickC.h>

extern "C"
{
#include <esp_err.h>
#include <esp_wifi.h>
#include <esp_event_loop.h>
}

unsigned long maxInactivityMillis = 20 * 1000;

void DeepSleepScreen::startSleep()
{
    //esp_sleep_enable_timer_wakeup( 15 * 1000* 1000 );
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);
    rtc_gpio_pulldown_dis( GPIO_NUM_37);
    rtc_gpio_pullup_en( GPIO_NUM_37);
    rtc_gpio_pulldown_dis( GPIO_NUM_39);
    rtc_gpio_pullup_en( GPIO_NUM_39);

    uint64_t mask = 0;
    mask = 687194767360; // GPIO37 and GPIO39
    Serial.printf("Mask = %llX\n", mask);
    esp_sleep_enable_ext1_wakeup( mask, ESP_EXT1_WAKEUP_ALL_LOW );

    M5.Lcd.print("Go to sleep\r\n");
    Serial.println("Go to sleep");    
    vTaskDelay(500);
    clear();
    esp_deep_sleep_start();
}

void DeepSleepScreen::onTimerTick()
{
    /*unsigned long now = millis();
    unsigned long inactivity = now - Screens.lastActivityMillis();
    if (inactivity > maxInactivityMillis)
    {
        //startSleep();
    }*/
}

void DeepSleepScreen::onSetup()
{
    startUITimer(500, true);
}

void DeepSleepScreen::onLongPress()
{
    startSleep();
}

void DeepSleepScreen::onRepaint()
{
    //clear();
    M5.Lcd.setCursor(0, 0, 1);

    M5.Lcd.printf("\r\n");
    M5.Lcd.printf("Deep Sleep\r\n");
    M5.Lcd.printf("\r\n");

    //unsigned long remainingSecs = (maxInactivityMillis - (millis() - Screens.lastActivityMillis())) / 1000;
    //M5.Lcd.printf("Remaining time:\r\n");
    //M5.Lcd.printf("%d seconds   \r\n", remainingSecs);

    M5.Lcd.printf("\r\nLong press:\r\n");
    M5.Lcd.printf("Sleep now\r\n");
    M5.Lcd.printf("\r\nWake up:\r\n");
    M5.Lcd.printf("Press the two botton\r\ntogether\r\n");
}
