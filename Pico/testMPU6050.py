import machine
from machine import Pin, I2C
import utime
import imu 

iic0 = I2C(0, sda=machine.Pin(20), scl=machine.Pin(21), freq=400000)
#print("I2C addr: ", i2c.scan()[0])
sensor = imu.MPU6050(iic0)

while True:
    #print(sensor.accel.xyz, sensor.gyro.xyz, sensor.temperature)
    print("加速度:{:7.2f}{:7.2f}{:7.2f};陀螺仪:{:9.2f}{:9.2f}{:9.2f};温度:{:5.1f}" \
          .format(sensor.accel.x, sensor.accel.y, sensor.accel.z,  \
                  sensor.gyro.x, sensor.gyro.y, sensor.gyro.z, \
                  sensor.temperature))
    utime.sleep(1)