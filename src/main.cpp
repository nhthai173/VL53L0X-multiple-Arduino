#include "Adafruit_VL53L0X.h"

// Địa chỉ I2C cho cảm biến
#define LOX1_ADDRESS 0x30
#define LOX2_ADDRESS 0x31

// Chân để reset cảm biến (nối với chân X)
#define SHT_LOX1 12
#define SHT_LOX2 13

Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();

VL53L0X_RangingMeasurementData_t measure1;
VL53L0X_RangingMeasurementData_t measure2;

/**
 * @brief Set ID for each sensor
 */
void setID() {
    // all reset
    digitalWrite(SHT_LOX1, LOW);
    digitalWrite(SHT_LOX2, LOW);
    delay(10);
    // all unreset
    digitalWrite(SHT_LOX1, HIGH);
    digitalWrite(SHT_LOX2, HIGH);
    delay(10);

    // activating LOX1 and resetting LOX2
    digitalWrite(SHT_LOX1, HIGH);
    digitalWrite(SHT_LOX2, LOW);

    // initing LOX1
    if(!lox1.begin(LOX1_ADDRESS)) {
        Serial.println("Failed to boot first VL53L0X sensor");
        while(1);
    }
    delay(10);

    // activating LOX2
    digitalWrite(SHT_LOX2, HIGH);
    delay(10);

    //initing LOX2
    if(!lox2.begin(LOX2_ADDRESS)) {
        Serial.println(F("Failed to boot second sensor"));
        while(1);
    }
}

void setup() {
    Serial.begin(9600);
    while (! Serial) delay(1);

    pinMode(SHT_LOX1, OUTPUT);
    pinMode(SHT_LOX2, OUTPUT);

    Serial.println(F("Starting..."));

    // Định địa chỉ cho cảm biến
    setID();

}

void loop() {

    lox1.rangingTest(&measure1, false);
    lox2.rangingTest(&measure2, false);

    /* ====== Main code here ====== */


    /* ====== Start Debug ====== */
    /* In thông số đo được ra màn hình */

    Serial.print("1: ");
    Serial.print(measure1.RangeMilliMeter);
    Serial.println(" mm");
    Serial.print("2: ");
    Serial.print(measure2.RangeMilliMeter);
    Serial.println(" mm");
    Serial.println();

    delay(100);
    /* ====== End Debug ====== */
}