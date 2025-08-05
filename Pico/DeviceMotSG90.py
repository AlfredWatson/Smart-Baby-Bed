import utime


def bed_on(sg90):
    #print("bed on")
    sg90.duty_u16(4095)
    utime.sleep(0.5)
    sg90.duty_u16(5734)
    utime.sleep(0.5)


def bed_off(sg90):
    #print("bed off")
    sg90.duty_u16(4915)
