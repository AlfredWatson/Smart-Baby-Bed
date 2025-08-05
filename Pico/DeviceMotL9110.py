import utime


def fan_on(l9110A, l9110B):
    #print("fan on")
    l9110A.value(1)
    l9110B.value(0)


def fan_off(l9110A, l9110B):
    #print("fan off")
    l9110A.value(0)
    l9110B.value(0)
