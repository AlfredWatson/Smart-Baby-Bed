class DeviceMotData:
    def __init__(self, steering_engine, small_fan, camera):
        # SG90
        self.steeringEngine = steering_engine
        # L9110
        self.smallFan = small_fan
        # ESP32-CAM
        self.camera = camera

    # set和get动作模块的运行信息
    # True:正在运行
    # False:不在运行
    def get_steering_engine(self):
        return self.steeringEngine

    def set_steering_engine(self, steering_engine):
        self.steeringEngine = steering_engine

    def get_small_fan(self):
        return self.smallFan

    def set_small_fan(self, small_fan):
        self.smallFan = small_fan

    def get_camera(self):
        return self.camera

    def set_camera(self, camera):
        self.camera = camera

    def __str__(self):
        executionModuleData = '{steeringEngine:' + str(self.steeringEngine) + ',' + \
                              'smallFan:' + str(self.smallFan) + ',' + \
                              'camera:' + str(self.camera) + '}'
        return executionModuleData