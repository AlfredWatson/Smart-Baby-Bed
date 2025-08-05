from machine import Pin, UART, ADC, PWM, I2C
#from utime import sleep
import utime
import _thread
from DHT22 import DHT22
from DeviceMotData import DeviceMotData
import imu
import DeviceMotSG90 as dm_sg90
import DeviceMotL9110 as dm_l9110


# pico-->esp01s的数据
sensorData = ''
# esp01s-->pico的数据
receiveData = ''
# 实例化动作执行对象
deviceMotInstance = DeviceMotData(False, False, True)
baby_cry = False
baby_cry_bed = False

# ---set up---
# 与ESP-01S通信
uart0 = UART(0, baudrate = 115200, tx = Pin(0), rx = Pin(1))
# 接收串口的杂乱信息
if uart0.any() != 0:
    print('receive:', uart0.read())

# DHT11
sensor_dht11_data = Pin(2, Pin.IN, Pin.PULL_UP)
# create DHT11 instance
sensor_dht11 = DHT22(sensor_dht11_data, dht11 = True)
dht11_t_old, dht11_h_old = 27,42
dht11_t_max = 100
dht11_t_flag = False

# MQ135
adc0 = ADC(Pin(26))
# digital in
sensor_mq135_din = Pin(3, Pin.IN, Pin.PULL_UP)
#adc2 = ADC(Pin(28))

# CZN-15E
adc1 = ADC(Pin(27))
# digital in
sensor_czn15e_din = Pin(4, Pin.IN, Pin.PULL_UP)

# SW-520D
sensor_sw520D_data = Pin(6, Pin.IN, Pin.PULL_UP)

# MH-RC
sensor_mhrc_din = Pin(7, Pin.IN, Pin.PULL_UP)

# LU96041
uart1 = UART(1, baudrate = 115200, tx = Pin(8), rx = Pin(9))
if uart1.any() != 0:
    print('receive:', uart1.read())

# L9110
l9110A = Pin(10, Pin.OUT)
l9110B = Pin(11, Pin.OUT)

# SG90
sg90 = PWM(Pin(12))
sg90.freq(50)

# MPU6050
iic0 = I2C(0, sda=machine.Pin(20), scl=machine.Pin(21), freq=400000)
#print("I2C addr: ", iic0.scan()[0])
sensor_mpu6050 = imu.MPU6050(iic0)
mpu_temp = round(sensor_mpu6050.temperature, 2)
#print(sensor_mpu6050.temperature)


def get_adc_analog_value(var_u16):
    #result = round((var_u16 - 12000) * 3.3 / 65535, 2)
    result = round(var_u16 * 3.3 / 65535, 2)
    return result


def mot_check():
    global deviceMotInstance
    while True:
        # bed
        if deviceMotInstance.get_steering_engine():
            dm_sg90.bed_on(sg90)
        else:
            dm_sg90.bed_off(sg90)
        # fan
        if deviceMotInstance.get_small_fan():
            dm_l9110.fan_on(l9110A, l9110B)
        else:
            dm_l9110.fan_off(l9110A, l9110B)
        '''
        if deviceMotInstance.get_camera():
            #print("cam on")
        else:
            #print("cam off")
        '''
        #print(deviceMotInstance.__str__())
        utime.sleep_ms(500)


def uart_receive_check():
    global baby_cry, baby_cry_bed, deviceMotInstance
    # 等待接收反馈
    break_count = 0;
    while True:
        if uart0.any() == 0:
            #print("didn\'t receive, wait.")
            break_count = break_count + 1
            if break_count == 10:
                break
        else:
            #binData = uart0.read()
            receiveData = uart0.read().decode() # .decode(): bytes-->str
            #print("receive:" + receiveData)
            
            # 判断串口接收到的信息
            if (receiveData[0] == '1'): # 表示esp已成功发布数据到MQTT
                receiveData = '' # clean received data
                break
            elif (receiveData[0] == 'm'): #'m:...'
                if (receiveData[2] == 'f'):
                    if (receiveData[7] == 'n'):# 'm:fan_on'
                        deviceMotInstance.set_small_fan(True)
                    else:# 'm:fan_off'
                        deviceMotInstance.set_small_fan(False)
                elif (receiveData[2] == 'b'):
                    if (receiveData[7] == 'n'):#'m:bed_on'
                        deviceMotInstance.set_steering_engine(True)
                    else:#'m:bed_off'
                        deviceMotInstance.set_steering_engine(False)
                        baby_cry_bed = False
                elif (receiveData[2] == 'c'):
                    if (receiveData[7] == 'n'):#'m:cam_on'
                        deviceMotInstance.set_camera(True)
                    else:
                        deviceMotInstance.set_camera(False)
            elif (receiveData[0] == 't'): #'t:_float_'
                dht11_t_max = float(receiveData[2:])
                #print("SET FAN ON WHEN:"+str(dht11_t_max))
                # 比较温度，打开风扇
                if dht11_t_old >= dht11_t_max:
                    deviceMotInstance.set_small_fan(True)
                else:
                    deviceMotInstance.set_small_fan(False)
            elif (receiveData[0] == 's'): #'s:...'
                if (receiveData[2] == 'y'): #'s:y'
                    #print("baby_cry_bed = True")
                    baby_cry_bed = True
                elif (receiveData[2] == 'n'): #'s:n'
                    #print("baby_cry_bed = False")
                    baby_cry_bed = False
                elif (receiveData[2] == '0'): #'s:0'
                    #print("baby_cry = True")
                    baby_cry = True
                else: #'s:...'
                    baby_cry = False
                # 两个都真才开摇床
                if (baby_cry and baby_cry_bed):
                    deviceMotInstance.set_steering_engine(True)
                else:
                    deviceMotInstance.set_steering_engine(False)
            # 收到的不是esp的发布确认而是控制信息,继续循环等待esp的发布确认,同时清空接收缓存
            receiveData = ''
        # if-else:end
        utime.sleep_ms(50)
    # while:end


if __name__ == '__main__':
    _thread.start_new_thread(mot_check, ())
    # ---loop---
    while True:
        sensorData = ''
        # SW-520D
        sensorData = sensorData + str(sensor_sw520D_data.value()) + ','
        # MH-RC
        sensorData = sensorData + str(sensor_mhrc_din.value()) + ','
        # LU96041
        tempChange = round(sensor_mpu6050.temperature, 2) - mpu_temp
        if tempChange > 0:
            bodytemp = 36.50 + tempChange * 0.1
        else:
            bodytemp = round(sensor_mpu6050.temperature, 2)
        mpu_temp = round(sensor_mpu6050.temperature, 2)
        sensorData = sensorData + str(bodytemp) + ','
        
        # DHT11
        dht11_t, dht11_h = sensor_dht11.read()
        if dht11_t is None:
            #print("dht11 sensor error!")
            dht11_t, dht11_h = dht11_t_old, dht11_h_old
        else:
            dht11_t_old, dht11_h_old = dht11_t, dht11_h
        sensorData = sensorData + str(dht11_t) + ',' + str(dht11_h) + ','
        
        # MQ-135
        adc0Value = get_adc_analog_value(adc0.read_u16())
        #adc0Value = adc0.read_u16()
        sensorData = sensorData + str(sensor_mq135_din.value()) + ',' + str(round(adc0Value, 2)) + ','
        
        # CZN-15E
        adc1Value = get_adc_analog_value(adc1.read_u16())
        #adc1Value = adc1.read_u16()
        sensorData = sensorData + str(sensor_czn15e_din.value()) + ',' + str(round(adc1Value, 2))
        
        # 检查一下
        print("send:" + sensorData)
        
        # Write the buffer of bytes to the bus.
        uart0.write(sensorData)
        # Waits until all data has been sent.
        uart0.flush()
        
        # 等待接收反馈
        uart_receive_check()
        
        # DHT22 not responsive if delay < 1s(1000ms)
        utime.sleep(6)

