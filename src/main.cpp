#include "Adafruit_VL53L0X.h"

// Địa chỉ I2C cho cảm biến
#define LOX1_ADDRESS 0x30
#define LOX2_ADDRESS 0x31
#define LOX3_ADDRESS 0x32

// Chân để reset cảm biến (nối với chân X)
#define SHT_LOX1 10
#define SHT_LOX2 12
#define SHT_LOX3 13

Adafruit_VL53L0X laser = Adafruit_VL53L0X();

VL53L0X_RangingMeasurementData_t measure;

/**
 * @brief Set ID for each sensor
 */
void setID() {
    // all reset
    digitalWrite(SHT_LOX1, LOW);
    digitalWrite(SHT_LOX2, LOW);
    digitalWrite(SHT_LOX3, LOW);
    delay(10);
    // all unreset
    digitalWrite(SHT_LOX1, HIGH);
    digitalWrite(SHT_LOX2, HIGH);
    digitalWrite(SHT_LOX3, HIGH);
    delay(10);

    // activating LOX1 and resetting LOX2
    digitalWrite(SHT_LOX1, HIGH);
    digitalWrite(SHT_LOX2, LOW);
    digitalWrite(SHT_LOX3, LOW);

    // initing LOX1
    if(!laser.begin(LOX1_ADDRESS)) {
        Serial.println("Failed to boot first VL53L0X sensor");
        while(1);
    }
    delay(10);

    // activating LOX2
    digitalWrite(SHT_LOX2, HIGH);
    delay(10);
    if(!laser.begin(LOX2_ADDRESS)) {
        Serial.println(F("Failed to boot second sensor"));
        while(1);
    }

    // activating LOX3
    digitalWrite(SHT_LOX3, HIGH);
    delay(10);
    if(!laser.begin(LOX3_ADDRESS)) {
        Serial.println(F("Failed to boot third sensor"));
        while(1);
    }
}

void setup() {
    Serial.begin(9600);
    while (! Serial) delay(1);

    pinMode(SHT_LOX1, OUTPUT);
    pinMode(SHT_LOX2, OUTPUT);
    pinMode(SHT_LOX3, OUTPUT);

    Serial.println(F("Starting..."));

    // Định địa chỉ cho cảm biến
    setID();

}

void loop() {

    uint16_t s1, s2, s3;

    /* ====== Đọc giá trị cảm biến ====== */
    
    // B1. Định địa chỉ
    laser.pMyDevice->I2cDevAddr = LOX1_ADDRESS & 0x7F;
    delay(10);
    // B2. Đọc giá trị
    laser.rangingTest(&measure, false);
    // B3. Lưu vào s1
    s1 = measure.RangeMilliMeter;

    laser.pMyDevice->I2cDevAddr = LOX2_ADDRESS & 0x7F;
    delay(10);
    laser.rangingTest(&measure, false);
    s2 = measure.RangeMilliMeter;

    laser.pMyDevice->I2cDevAddr = LOX3_ADDRESS & 0x7F;
    delay(10);
    laser.rangingTest(&measure, false);
    s3 = measure.RangeMilliMeter;


    /* ====== Main code here ====== */
    
    // In giá trị cảm biến 1
    Serial.print("1: ");
    Serial.print(s1);
    Serial.println(" mm");

    // In giá trị cảm biến 2
    Serial.print("2: ");
    Serial.print(s2);
    Serial.println(" mm");

    // In giá trị cảm biến 3
    Serial.print("3: ");
    Serial.print(s3);
    Serial.println(" mm");
    
    Serial.println();

    delay(100);
}