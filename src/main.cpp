#include "APP_CFG.h"
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include "HAL/GPIO/GPIO.h"
#include "APP/LDR/LDR.h"
#include "APP/LM35/LM35.h"
#include "APP/IMU/IMU.h"
#include "APP/BLYNK/BlynkApp.h"
#include "APP/ULTRASOUND/ultra.h"
#include <BlynkSimpleEsp32.h>

void BLYNK_MAIN (void);

static bool activate_systems=0;
static GPIO_t blinking_led= {.pin_num = 2, .pin_mode=OUTPUT,.pin_value=0};

struct APP_DATA
{
    uint16_t LDR_value_Data;
    double LM35_Data;
    double accel_x_Data;
    double accel_y_Data;
    double accel_z_Data;
    double gyro_x_Data; 
    double gyro_y_Data; 
    double gyro_z_Data; 
    double imu_tempr_Data;
    float ultra_sonic_Data;
}DATA;


char SSID[]    = "realme 10";
char PASWORD[] = "123456789";

BlynkTimer timer;

void setup(void)
{
  Serial.begin(UART0_BAUD_RATE);
  Blynk.begin(BLYNK_AUTH_TOKEN, SSID, PASWORD);
  GPIO_initPin(blinking_led);
  LDR_Init();
  LM35_init();
  IMU_init();
  ultra_init();
  timer.setInterval(BLYNK_STREAM_TIME, BLYNK_MAIN);
}

void loop()
{
  // uint16_t u16_ldr_value = LDR_ReadRaw();
  // double d_lm35_temp = 0.0;
  // IMU_updateValue();
  // sensors_event_t imu_accel_values = IMU_getAccMeasur();
  // double accel_x = imu_accel_values.acceleration.x;
  // double accel_y = imu_accel_values.acceleration.y;
  // double accel_z = imu_accel_values.acceleration.z;
  // sensors_event_t imu_gyro_values = IMU_getGyroMeasur();
  // double gyro_x = imu_gyro_values.gyro.x;
  // double gyro_y = imu_gyro_values.gyro.y;
  // double gyro_z = imu_gyro_values.gyro.z;
  // sensors_event_t imu_tempr_valurs =  IMU_getTempMeasur();
  // double imu_tempr = imu_tempr_valurs.temperature;
  // LM35_update();
  // LM35_getTemp(&d_lm35_temp);
Blynk.run();
timer.run();

}

BLYNK_WRITE(V5)
{
  activate_systems =param.asInt();
}

BLYNK_WRITE(V1)
{
  blinking_led.pin_value=param.asInt();
  GPIO_writePinValue(blinking_led);
}

void BLYNK_MAIN (void)
{
  if (activate_systems)
  {
    DATA.LDR_value_Data = LDR_ReadRaw();
    
    IMU_updateValue();
    
    sensors_event_t imu_accel_values = IMU_getAccMeasur();
    
    DATA.accel_x_Data = imu_accel_values.acceleration.x;
    DATA.accel_y_Data = imu_accel_values.acceleration.y;
    DATA.accel_z_Data = imu_accel_values.acceleration.z;
    
    
    sensors_event_t imu_gyro_values = IMU_getGyroMeasur();
    
    DATA.gyro_x_Data = imu_gyro_values.gyro.x;
    DATA.gyro_y_Data = imu_gyro_values.gyro.y;
    DATA.gyro_z_Data = imu_gyro_values.gyro.z;
    
    
    sensors_event_t imu_tempr_valurs =  IMU_getTempMeasur();
    
    DATA.imu_tempr_Data = imu_tempr_valurs.temperature;
    
    DATA.ultra_sonic_Data = ultra_measureDistance();
    Serial.print("Ultrasonic Distance: ");
    Serial.print(DATA.ultra_sonic_Data);
    Serial.println(" cm");
    
    LM35_update();
    LM35_getTemp(DATA.LM35_Data);

    Blynk.virtualWrite(V0,DATA.LM35_Data);
    Blynk.virtualWrite(V2,DATA.LDR_value_Data);
    Blynk.virtualWrite(V3,DATA.accel_x_Data);
    Blynk.virtualWrite(V4,DATA.gyro_x_Data);
    Blynk.virtualWrite(V8,DATA.ultra_sonic_Data);
  }
}