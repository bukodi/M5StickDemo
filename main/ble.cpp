#include <M5StickC.h>
#include "ble.h"

/*
    Video: https://www.youtube.com/watch?v=oCMOYS71NIU
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleNotify.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updated by chegewara

   Create a BLE server that, once we receive a connection, will send periodic notifications.
   The service advertises itself as: 4fafc201-1fb5-459e-8fcc-c5c9c331914b
   And has a characteristic of: beb5483e-36e1-4688-b7f5-ea07361b26a8

   The design of creating the BLE server is:
   1. Create a BLE Server
   2. Create a BLE Service
   3. Create a BLE Characteristic on the Service
   4. Create a BLE Descriptor on the characteristic
   5. Start the service.
   6. Start advertising.

   A connect hander associated with the server starts a background task that performs notification
   every couple of seconds.
*/
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLEServer *pServer = NULL;
BLEService *pService = NULL;
BLECharacteristic *pChar_fidoControlPoint = NULL;
BLECharacteristic *pChar_fidoStatus = NULL;
BLECharacteristic *pChar_fidoControlPointLength = NULL;
BLECharacteristic *pChar_fidoServiceRevisionBitfield = NULL;
BLECharacteristic *pChar_fidoServiceRevision = NULL;

bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t value = 0;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define FIDO_SERVICE_UUID "fffd"
//#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
//#define CHARACTERISTIC_UUID     "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define UUID_fidoControlPoint                "F1D0FFF1-DEAA-ECEE-B42F-C9BA7ED623BB"
#define UUID_fidoStatus                      "F1D0FFF2-DEAA-ECEE-B42F-C9BA7ED623BB"
#define UUID_fidoControlPointLength          "F1D0FFF3-DEAA-ECEE-B42F-C9BA7ED623BB"
#define UUID_fidoServiceRevisionBitfield     "F1D0FFF4-DEAA-ECEE-B42F-C9BA7ED623BB"
#define UUID_fidoServiceRevision             "2A28"

class MyServerCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer *pServer)
    {
        deviceConnected = true;
        Serial.println("Client connected.");

    };

    void onDisconnect(BLEServer *pServer)
    {
        deviceConnected = false;
        Serial.println("Client diconnected.");
    }
};

class MyCharacteristicCallbacks : public BLECharacteristicCallbacks
{
	void onRead(BLECharacteristic* pCharacteristic) {
        BLEUUID uuid = pCharacteristic->getUUID();
    }
	void onWrite(BLECharacteristic* pCharacteristic) {

    }
    void onConnect(BLEServer *pServer)
    {
        deviceConnected = true;
        BLEDevice::startAdvertising();
        Serial.println("Client connected.");

    };

    void onDisconnect(BLEServer *pServer)
    {
        deviceConnected = false;
        Serial.println("Client diconnected.");
    }
};

void BleScreen::onSetup()
{
    // Create the BLE Device
    BLEDevice::init("M5Stick Authenticator");

    // Create the BLE Server
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    // Create the BLE Service
    BLEService *pService = pServer->createService(FIDO_SERVICE_UUID);

    // Create a BLE Characteristics
    pChar_fidoControlPoint = pService->createCharacteristic(
        UUID_fidoControlPoint,
        BLECharacteristic::PROPERTY_WRITE);
    //pChar_fidoControlPoint->setCallbacks()
    pChar_fidoStatus = pService->createCharacteristic(
        UUID_fidoStatus,
        BLECharacteristic::PROPERTY_NOTIFY);
    pChar_fidoControlPointLength = pService->createCharacteristic(
        UUID_fidoControlPointLength,
        BLECharacteristic::PROPERTY_READ);
    pChar_fidoServiceRevisionBitfield = pService->createCharacteristic(
        UUID_fidoServiceRevisionBitfield,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
    pChar_fidoServiceRevision = pService->createCharacteristic(
        UUID_fidoServiceRevision,
        BLECharacteristic::PROPERTY_READ);

    // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
    // Create a BLE Descriptor
    // pChar_fidoControlPoint->addDescriptor(new BLE2902());

    // Start the service
    pService->start();

    // Start advertising
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(FIDO_SERVICE_UUID);
    pAdvertising->setScanResponse(false);
    pAdvertising->setMinPreferred(0x0); // set value to 0x00 to not advertise this parameter
    BLEDevice::startAdvertising();
    Serial.println("Waiting a client connection to notify...");
}

void startFIDOService() {
    // Create the BLE Device
    BLEDevice::init("M5Stick Authenticator");

    // Create the BLE Server
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    // Create the BLE Service
    pService = pServer->createService(FIDO_SERVICE_UUID);

    // Create a BLE Characteristics
    pChar_fidoControlPoint = pService->createCharacteristic(
        UUID_fidoControlPoint,
        BLECharacteristic::PROPERTY_WRITE);
    //pChar_fidoControlPoint->setCallbacks()
    pChar_fidoStatus = pService->createCharacteristic(
        UUID_fidoStatus,
        BLECharacteristic::PROPERTY_NOTIFY);
    pChar_fidoControlPointLength = pService->createCharacteristic(
        UUID_fidoControlPointLength,
        BLECharacteristic::PROPERTY_READ);
    pChar_fidoServiceRevisionBitfield = pService->createCharacteristic(
        UUID_fidoServiceRevisionBitfield,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
    pChar_fidoServiceRevision = pService->createCharacteristic(
        UUID_fidoServiceRevision,
        BLECharacteristic::PROPERTY_READ);

    // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
    // Create a BLE Descriptor
    // pChar_fidoControlPoint->addDescriptor(new BLE2902());

    // Start the service
    pService->start();

    // Start advertising
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(FIDO_SERVICE_UUID);
    pAdvertising->setScanResponse(false);
    pAdvertising->setMinPreferred(0x0); // set value to 0x00 to not advertise this parameter
    BLEDevice::startAdvertising();
    Serial.println("Waiting a client connection to notify...");
}

void stopFIDOService() {

    // Create the BLE Server
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    // Create the BLE Service
    BLEService *pService = pServer->createService(FIDO_SERVICE_UUID);

    // Create a BLE Characteristics
    pChar_fidoControlPoint = pService->createCharacteristic(
        UUID_fidoControlPoint,
        BLECharacteristic::PROPERTY_WRITE);
    //pChar_fidoControlPoint->setCallbacks()
    pChar_fidoStatus = pService->createCharacteristic(
        UUID_fidoStatus,
        BLECharacteristic::PROPERTY_NOTIFY);
    pChar_fidoControlPointLength = pService->createCharacteristic(
        UUID_fidoControlPointLength,
        BLECharacteristic::PROPERTY_READ);
    pChar_fidoServiceRevisionBitfield = pService->createCharacteristic(
        UUID_fidoServiceRevisionBitfield,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
    pChar_fidoServiceRevision = pService->createCharacteristic(
        UUID_fidoServiceRevision,
        BLECharacteristic::PROPERTY_READ);

    // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
    // Create a BLE Descriptor
    // pChar_fidoControlPoint->addDescriptor(new BLE2902());


    // Stop advertising
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->stop();

    pService->stop();
    pService->~BLEService();
    pService = NULL;


    pServer->~BLEServer();
    pServer = NULL;

        // Create the BLE Device
    BLEDevice::deinit();
    Serial.println("BLE service stoped.");

}

void bleloop()
{
    // notify changed value
    if (deviceConnected)
    {
        pChar_fidoStatus->setValue((uint8_t *)&value, 4);
        pChar_fidoStatus->notify();
        value++;
        delay(10); // bluetooth stack will go into congestion, if too many packets are sent, in 6 hours test i was able to go as low as 3ms
    }
    // disconnecting
    if (!deviceConnected && oldDeviceConnected)
    {
        delay(500);                  // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected)
    {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
}

void BleScreen::onLongPress()
{
    M5.Lcd.printf("\r\nBLE longpress\r\n");
}

void BleScreen::onRepaint()
{
    bleloop();
    clear();
    M5.Lcd.printf("\r\nPlace for\r\nBLE settings\r\n");
}
