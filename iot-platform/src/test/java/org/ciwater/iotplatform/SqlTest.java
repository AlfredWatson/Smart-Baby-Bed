package org.ciwater.iotplatform;

import org.ciwater.iotplatform.dao.DeviceMapper;
import org.ciwater.iotplatform.dao.RoleMapper;
import org.ciwater.iotplatform.dao.SensorArrayDataMapper;
import org.ciwater.iotplatform.dao.UserMapper;
import org.ciwater.iotplatform.service.*;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;

import javax.annotation.Resource;

/**
 * @author AlfredWatson
 * @create 2021-12-28 16:35
 */
@SpringBootTest
public class SqlTest {

//    @Resource
//    UserService service;
//
//    @Test
//    public void test2(){
//        System.out.println(service.getUserByName("haojifei"));
//    }
//
//    RoleMapper mapper;
//
//    @Autowired
//    public void setMapper(RoleMapper mapper) {
//        this.mapper = mapper;
//    }
//
//    @Test
//    public void test03(){
//        System.out.println(mapper.getAllRoles());
//    }

//    UserMapper mapper;
//
//    @Autowired
//    public void setMapper(UserMapper mapper) {
//        this.mapper = mapper;
//    }
//
//    @Test
//    public void test03() {
//        System.out.println(mapper.setUser("daE", "eee", "404@cimail.org", true));
//    }

    RoleMapper mapper;

    @Autowired
    public void setMapper(RoleMapper mapper) {
        this.mapper = mapper;
    }

    @Test
    public void test03() {
        System.out.println(mapper.setUserRoles(3,4));
    }
}
