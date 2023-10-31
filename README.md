## Sử dụng nhiều module VL53L0X trên 1 kênh I2C với Arduino

### 1. Kết nối

![](./assets/2_VL53L0X_Arduino.jpg)

- Cấp nguồn +5V và GND cho các module VL53L0X
- Chân SCL được nối với nhau và nối với SCL trên Arduino
- Chân SDA được nối với nhau và nối với SDA trên Arduino
- Chân X (XSHUT) được nối với các chân Digital trên Arduino để điều khiển bật tắt module VL53L0X

### 2. Cài đặt và sử dụng

- Sử dụng thư viện [Adafruit_VL53L0X](https://www.arduino.cc/reference/en/libraries/adafruit_vl53l0x/) nhưng đã có chỉnh sửa
- Trong ví dụ này sử dụng PlatformIO
- Clone repo này về và mở bằng PlatformIO để chạy

### 3. Nguyên lý hoạt động

- Module VL53L0X có chân XSHUT để điều khiển bật tắt module. Khi chân XSHUT được kéo xuống mức LOW thì module sẽ bị tắt nguồn. Khi chân XSHUT được kéo lên mức HIGH thì module sẽ được cấp nguồn.
- Để sử dụng được nhiều module thì ta cần định các địa chỉ I2C khác nhau cho chúng. Để làm điều này, ta sẽ sử dụng chân XSHUT để điều khiển bật tắt module. Mỗi khi module được tắt đi và bật lại thì nó sẽ về địa chỉ mặc định là `0x29`. Ta chỉ có thể đổi địa chỉ cho cùng lúc 1 module nên vì vậy các bước để đổi địa chỉ như sau:

1. Reset về địa chỉ mặc định bằng cách tắt nguồn module và bật lại rồi lại tắt tất cả module.
2. Lần lượt bật từng module lên và đổi địa chỉ cho từng module.
   - Địa chỉ được đặt phải khác địa chỉ mặc định
   - Địa chỉ của các module phải khác nhau

Như vậy ta có thể sử dụng nhiều module VL53L0X trên 1 kênh I2C với Arduino.

### 4. Giải thích code

Trong ví dụ này sẽ demo với 3 cảm biến.

1. Đặt các địa chỉ cho các module

```cpp
#define LOX1_ADDRESS 0x30
#define LOX2_ADDRESS 0x31
#define LOX3_ADDRESS 0x32
```

2. Khải báo các chân XSHUT

```cpp
#define SHT_LOX1 10
#define SHT_LOX2 12
#define SHT_LOX3 13
```

3. Khởi tạo các biến

```cpp
// Các biến quản lý các module
Adafruit_VL53L0X laser = Adafruit_VL53L0X();

// Các biến lưu trữ giá trị của các cảm biến
VL53L0X_RangingMeasurementData_t measure;
```

4. Chỉnh sửa hàm `setID()` để phù hợp với số lượng module sử dụng
5. Khai báo OUTPUT cho các chân XSHUT, và gọi hàm `setID()` để đặt địa chỉ cho các module trong hàm `setup()`

```cpp
void setup() {
    pinMode(SHT_LOX1, OUTPUT);
    pinMode(SHT_LOX2, OUTPUT);
    pinMode(SHT_LOX3, OUTPUT);

    // Định địa chỉ cho cảm biến
    setID();
}
```

6. Trong hàm `loop()` đọc giá trị cảm biến

```cpp
void loop() {
    // B1. Định địa chỉ
    laser.pMyDevice->I2cDevAddr = LOX1_ADDRESS & 0x7F;
    delay(10); // Thời gian chờ ổn định - có thể để nhỏ hơn
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
}
```

### Build và chạy

![1698743715710](image/README/1698743715710.png)

Thời gian đáp ứng: delay có thể chỉnh nhỏ hơn nữa

```cpp
laser.pMyDevice->I2cDevAddr = LOX2_ADDRESS & 0x7F;
delay(10); <---
laser.rangingTest(&measure, false);
s2 = measure.RangeMilliMeter;
```
