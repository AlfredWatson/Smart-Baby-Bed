package org.ciwater.iotplatform.service;

import com.fasterxml.jackson.databind.ObjectMapper;
import org.ciwater.iotplatform.dao.SensorArrayDataMapper;
import org.ciwater.iotplatform.pojo.SensorArrayData;
import org.ciwater.iotplatform.pojo.SensorArrayDataSub;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.math.BigDecimal;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;

/**
 * @author AlfredWatson
 * @create 2021-12-28 16:39
 */
@Service
public class SensorArrayDataServiceImpl implements SensorArrayDataService {

    SensorArrayDataMapper mapper;

    @Autowired
    public void setMapper(SensorArrayDataMapper mapper) {
        this.mapper = mapper;
    }

    /**
     * 在传感器数据库中，根据设备id查出在时间区间[start,end]内的所有数据项
     *
     * @param id:    设备id
     * @param start: 从start时间开始
     * @param end:   到end时间结束
     * @return 传感器阵列数据列表
     */
    @Override
    public List<SensorArrayData> getTSSADataByDevId(int id, BigDecimal start, BigDecimal end) {
        System.out.println("service:getTSSADataByDevId");
        System.out.println("设备:" + id + " 开始:" + start + " 结束:" + end);
        List<SensorArrayData> list = mapper.getTSSADataByDevId(id, start, end);
//        System.out.println("返回" + list);
        return list;
    }

    /**
     * 在传感器数据库中，根据设备id查出all条数据(从第start条开始)
     *
     * @param id:    设备数据库id
     * @param start: 传感器数据库第start条
     * @param all:   一共第all条
     * @return 传感器阵列数据列表
     */
    @Override
    public List<SensorArrayData> getNUMSADataByDevId(int id, int start, int all) {
        System.out.println("service:getNUMSADataByDevId");
        System.out.println("设备:" + id + " 开始:" + start + " 全部:" + all);
        List<SensorArrayData> list = mapper.getNUMSADataByDevId(id, start, all);
//        System.out.println("返回" + list);
        return list;
    }

    @Override
    public List<SensorArrayDataSub> getAllSubData() {
        System.out.println("物联网service:getAll");
        List<SensorArrayDataSub> list = mapper.getAllSubData();
        return list;
    }

    @Override
    public void saveAllSubData(String payload) {
        System.out.println("物联网service:saveAll");
        ObjectMapper mapper = new ObjectMapper();
        try {
            SensorArrayDataSub dataSub = mapper.readValue(payload, SensorArrayDataSub.class);
            System.out.println("转换为对象" + dataSub.toString());
            Date day = new Date();
            SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
            System.out.println("日期" + dateFormat.format(day));
            this.mapper.saveAllSubData(dateFormat.format(day),
                    dataSub.getDeviceId(),
                    dataSub.getSensorTemp(),
                    dataSub.getSensorHumidity());
        } catch (Exception e) {
            System.out.println("json转换为对象失败!!!");
            e.printStackTrace();
        }
    }
}
