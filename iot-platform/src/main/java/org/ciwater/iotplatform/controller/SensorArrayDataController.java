package org.ciwater.iotplatform.controller;

import org.ciwater.iotplatform.dao.SensorArrayDataMapper;
import org.ciwater.iotplatform.pojo.SensorArrayData;
import org.ciwater.iotplatform.pojo.SensorArrayDataSub;
import org.ciwater.iotplatform.service.SensorArrayDataService;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.*;

import javax.annotation.Resource;
import javax.servlet.http.HttpServletRequest;
import java.math.BigDecimal;
import java.util.List;
import java.util.Map;

/**
 * @author AlfredWatson
 * @create 2021-12-28 17:10
 */
@Controller
public class SensorArrayDataController {
    @Resource
    private SensorArrayDataService service;

    @ResponseBody
    @GetMapping("/getNUMSAData")
    public List<SensorArrayData> getNUMSADataByDevId(Integer id, Integer start, Integer end){
        System.out.println("controller:getNUMSADataByDevId");
        return service.getNUMSADataByDevId(2, 3,5);
    }


//    @ResponseBody
//    @GetMapping("/getTSSAData")
//    public List<SensorArrayDataMapper> getTSSADataByDevId(Integer id, BigDecimal start, BigDecimal end) {
//        System.out.println("controller:getTSSADataByDevId");
//        return service.getTSSADataByDevId(1, new BigDecimal(1594512101), new BigDecimal(1594532253));
//    }

    @ResponseBody
    @RequestMapping("/getTSSAData")
    public List<SensorArrayData> getTSSADataByDevId() {
        System.out.println("controller:getTSSADataByDevId");
        return service.getTSSADataByDevId(2, new BigDecimal(1594512101), new BigDecimal(1594512953));
    }

    //    @ResponseBody
//    @RequestMapping("/getTSSAData2")
//    public List<SensorArrayDataMapper> getTSSADataByDevId2(@PathVariable("p1") String id,
//                                                           @PathVariable("p2") String start,
//                                                           @PathVariable("p3") String period) {
//        System.out.println("controller:getTSSADataByDevId");
//        System.out.println("ttt:"+id+"yyy:"+start+"uuu:"+period);
//        return service.getTSSADataByDevId(1, new BigDecimal(1594512101), new BigDecimal(1594522253));
//    }
    @ResponseBody
    @RequestMapping("/getTSSAData2")
    public List<SensorArrayData> getTSSADataByDevId2() {
        System.out.println("controller:getTSSADataByDevId");
        return service.getTSSADataByDevId(1, new BigDecimal(1594512101), new BigDecimal(1594512111));
    }

    @ResponseBody
    @RequestMapping("/getAllSubData")
    public List<SensorArrayDataSub> getAllSubData() {
        System.out.println("物联网controller:getAllSubData");
        List<SensorArrayDataSub> list = service.getAllSubData();
        System.out.println(list);
        return list;
    }
}
