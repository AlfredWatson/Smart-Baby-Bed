package org.ciwater.iotplatform.service;

import org.ciwater.iotplatform.pojo.Device;
import org.springframework.stereotype.Service;

import java.util.List;

/**
 * @author AlfredWatson
 * @create 2021-12-26 18:36
 */
@Service
public interface DeviceService {
    List<Device> getAllDevice();

    String getDeviceMac(int id);

    int getDeviceId(String mac);
}
