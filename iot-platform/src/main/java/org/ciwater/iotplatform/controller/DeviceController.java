package org.ciwater.iotplatform.controller;

import org.ciwater.iotplatform.pojo.Device;
import org.ciwater.iotplatform.service.DeviceService;
import org.ciwater.iotplatform.service.DeviceServiceImpl;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.ResponseBody;

import javax.annotation.Resource;
import java.util.List;
import java.util.Map;

/**
 * @author AlfredWatson
 * @create 2021-12-27 13:22
 */
@Controller
public class DeviceController {

    @Resource
    private DeviceService service;

    @ResponseBody
    @GetMapping("/getDevices")
    public List<Device> getAllDevice(){
        System.out.println("controller:getAllDevice");
        List<Device> list = service.getAllDevice();
//        map.put("devices", list);
        return list;
    }

    @ResponseBody
    @GetMapping("/getDeviceMac")
    public String getDeviceMac(int id){
        System.out.println("controller:getDeviceMac");
        return service.getDeviceMac(id);
    }

    @ResponseBody
    @GetMapping("/getDeviceId")
    public int getDeviceId(String mac){
        System.out.println("controller:getDeviceId");
        return service.getDeviceId(mac);
    }
}
