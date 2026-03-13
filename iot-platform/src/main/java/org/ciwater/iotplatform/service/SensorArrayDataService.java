package org.ciwater.iotplatform.service;

import org.ciwater.iotplatform.dao.SensorArrayDataMapper;
import org.ciwater.iotplatform.pojo.SensorArrayData;
import org.ciwater.iotplatform.pojo.SensorArrayDataSub;
import org.springframework.stereotype.Service;

import java.math.BigDecimal;
import java.util.List;

/**
 * @author AlfredWatson
 * @create 2021-12-28 16:36
 */
@Service
public interface SensorArrayDataService {
    /**
     * 在传感器数据库中，根据设备id查出在时间区间[start,end]内的所有数据项
     * @param id: 设备id
     * @param start: 从start时间开始
     * @param end: 到end时间结束
     * @return 传感器阵列数据列表
     */
    List<SensorArrayData> getTSSADataByDevId(int id, BigDecimal start, BigDecimal end);

    /**
     * 在传感器数据库中，根据设备id查出all条数据(从第start条开始)
     * @param id: 设备数据库id
     * @param start: 传感器数据库第start条
     * @param all: 一共第all条
     * @return 传感器阵列数据列表
     */
    List<SensorArrayData> getNUMSADataByDevId(int id, int start, int all);

    List<SensorArrayDataSub> getAllSubData();
    void saveAllSubData(String payload);
}
