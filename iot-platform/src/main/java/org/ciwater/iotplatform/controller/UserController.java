package org.ciwater.iotplatform.controller;

import org.ciwater.iotplatform.service.MailServiceImpl;
import org.ciwater.iotplatform.service.RoleService;
import org.ciwater.iotplatform.service.UserService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.mail.javamail.JavaMailSenderImpl;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestParam;

import javax.annotation.Resource;
import java.util.Map;

/**
 * @author AlfredWatson
 * @create 2021-12-30 13:15
 */
@Controller
public class UserController {

    @Resource
    private UserService userService;
    @Resource
    private RoleService roleService;
    @Resource
    private MailServiceImpl mailService;

    @PostMapping("/register")
    public String userRegister(@RequestParam("username") String name,
                               @RequestParam("password") String password,
                               @RequestParam("mail") String mail,
                               @RequestParam("verCode") String verificationCode,
                               Model model){

//        if (verificationCode.equals()){
//
//        } else {
//            model.addAttribute("msg", "验证码错误!");
//            return "/public/register";
//        }
        return null;
    }
}
