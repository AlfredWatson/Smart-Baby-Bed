package org.ciwater.iotplatform.controller;

import org.ciwater.iotplatform.pojo.Device;
import org.ciwater.iotplatform.service.DeviceService;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;

import javax.annotation.Resource;
import java.util.List;

/**
 * @author AlfredWatson
 * @create 2022-01-18 16:06
 */
@Controller
public class MyTestController {
    @Resource
    private DeviceService service;

    // 测试
    @RequestMapping("/h1f")
    public String staticHtml2(@RequestParam(name = "email") String string1,
                              @RequestParam(name = "password") String string2,
                              Model model) {
        System.out.println(string1 + "[]" + string2);
        if (string1.equals("123")) {
            model.addAttribute("loginMsg", "大手笔");
            System.out.println("123");
        } else {
            System.out.println("456");
            model.addAttribute("loginMsg", "小市民");
        }
        return "/user/user-index";
    }

    // 测试
    @ResponseBody
    @RequestMapping("/sendDateTime")
    public List<Device> staticHtm3l(@RequestParam(name = "foo") String userFoo,
                                    @RequestParam(name = "myFuckingDate") String date) {
        System.out.println("佛欧:" + userFoo + " 日期:" + date);
        return service.getAllDevice();
    }

    // 测试
    @ResponseBody
    @RequestMapping("/test008")
    public String test008(@RequestParam(name = "float") String xiaoShu,
                          @RequestParam(name = "int") String zhengShu) {
        System.out.println("x:" + xiaoShu + " y:" + zhengShu);
        return "556";
    }

    // 测试
    @ResponseBody
    @RequestMapping("/test009")
    public String test009(@RequestParam(name = "flag") String f) {
        System.out.print("[ok1: " + f);
        int a = Integer.parseInt(f);
        System.out.println(" ok2: " + a + "]");
        if (a > 0) {
            return "flag: 1";
        } else {
            return "flag: -1";
        }
    }

    @ResponseBody
    @RequestMapping("/showDNM")
    public String test01() {
        System.out.println("我被8266搞了");
        return "dong an ming 真帅!";
    }

}
