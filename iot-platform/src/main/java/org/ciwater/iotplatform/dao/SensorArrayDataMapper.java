package org.ciwater.iotplatform.dao;

import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;
import org.ciwater.iotplatform.pojo.SensorArrayData;
import org.ciwater.iotplatform.pojo.SensorArrayDataSub;
import org.springframework.stereotype.Repository;

import java.math.BigDecimal;
import java.util.List;

/**
 * @author AlfredWatson
 * @create 2021-12-27 17:27
 */
@Mapper
@Repository
public interface SensorArrayDataMapper {
    /**
     * 在传感器数据库中，根据设备id查出在时间区间[start,end]内的所有数据项
     *
     * @param id:    设备id
     * @param start: 从start时间开始
     * @param end:   到end时间结束
     * @return 传感器阵列数据列表
     */
    List<SensorArrayData> getTSSADataByDevId(@Param("dev_id") int id,
                                             @Param("time_start") BigDecimal start,
                                             @Param("time_end") BigDecimal end);

    /**
     * 在传感器数据库中，根据设备id查出all条数据(从第start条开始)
     *
     * @param id:    设备数据库id
     * @param start: 传感器数据库第start条
     * @param all:   一共第all条
     * @return 传感器阵列数据列表
     */
    List<SensorArrayData> getNUMSADataByDevId(@Param("dev_id") int id,
                                              @Param("item_str") int start,
                                              @Param("item_all") int all);

    List<SensorArrayDataSub> getAllSubData();

    void saveAllSubData(@Param("p1") String utcTime,
                        @Param("p2")int deviceId,
                        @Param("p3")BigDecimal sensorTemp,
                        @Param("p4")BigDecimal sensorHumidity);
}
