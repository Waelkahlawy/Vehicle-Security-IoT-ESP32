#ifndef IMU_H
#define IMU_H

#include <Adafruit_Sensor.h>

void IMU_init(void);

void IMU_updateValue(void);

sensors_event_t IMU_getAccMeasur(void);

sensors_event_t IMU_getGyroMeasur(void);

sensors_event_t IMU_getTempMeasur(void);




#endif //IMU_H