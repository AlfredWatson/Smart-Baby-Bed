package org.ciwater.iotplatform.service;

import org.ciwater.iotplatform.dao.DeviceMapper;
import org.ciwater.iotplatform.pojo.Device;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;

/**
 * @author AlfredWatson
 * @create 2021-12-27 13:19
 */
@Service
public class DeviceServiceImpl implements DeviceService {

    DeviceMapper mapper;

    @Autowired
    public void setMapper(DeviceMapper mapper){
        this.mapper = mapper;
    }

    @Override
    public List<Device> getAllDevice() {
        System.out.println("service:getAllDevice");
        return mapper.getAllDevice();
    }

    @Override
    public String getDeviceMac(int id) {
        System.out.println("service:getDeviceMac");
        return mapper.getDeviceMac(id);
    }

    @Override
    public int getDeviceId(String mac) {
        System.out.println("service:getDeviceId");
        return mapper.getDeviceId(mac);
    }
}
