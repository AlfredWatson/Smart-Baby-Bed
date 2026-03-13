package org.ciwater.iotplatform.controller;

import org.ciwater.iotplatform.pojo.Device;
import org.ciwater.iotplatform.service.DeviceService;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.*;

import javax.annotation.Resource;
import java.util.List;
import java.util.Map;

/**
 * @author AlfredWatson
 * @create 2021-12-27 13:44
 */
@Controller
public class RouterController {

    @Resource
    private DeviceService service;

    @RequestMapping({"/", "/index"})
    public String index(Map<String, Object> map) {
        map.put("devices", service.getAllDevice());
        return "/admin/admin-index";
    }

    // 登录
    @RequestMapping("/toLogin")
    public String toLogin() {
        return "/public/login";
    }

    // 注册
    @RequestMapping("/toRegister")
    public String toRegister() {
        return "/public/register";
    }

    // 测试
    @RequestMapping("/h1")
    public String staticHtml() {
        return "/user/test";
    }
    // 物联网工程实践测试
    @RequestMapping("/h2")
    public String staticHtml2() {
        return "/user/test2";
    }
}
