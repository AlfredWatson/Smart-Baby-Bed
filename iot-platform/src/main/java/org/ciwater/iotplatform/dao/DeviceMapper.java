package org.ciwater.iotplatform.dao;

import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;
import org.ciwater.iotplatform.pojo.Device;
import org.springframework.stereotype.Repository;

import java.util.List;

/**
 * @author AlfredWatson
 * @create 2021-12-26 18:36
 */
@Mapper
@Repository
public interface DeviceMapper {
    /**
     * 获得所有设备信息
     * @return 所有设备的列表
     */
    List<Device> getAllDevice();

    /**
     * 根据设备id获得设备MAC地址
     * @param id: 设备id
     * @return 设备MAC地址
     */
    String getDeviceMac(@Param("dev_id") int id);

    /**
     * 根据设备MAC地址获得设备id
     * @param mac: 设备MAC地址
     * @return 设备id
     */
    int getDeviceId(@Param("dev_mac") String mac);
}
