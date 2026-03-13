package org.ciwater.iotplatform.service;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.mail.SimpleMailMessage;
import org.springframework.mail.javamail.JavaMailSenderImpl;
import org.springframework.stereotype.Service;

/**
 * @author AlfredWatson
 * @create 2021-12-30 14:10
 */
@Service
public class MailServiceImpl {

    JavaMailSenderImpl mailSender;

    @Autowired
    public void setMailSender(JavaMailSenderImpl mailSender) {
        this.mailSender = mailSender;
    }

    public void sendVerificationCode(String verificationCode, String mail) {
        SimpleMailMessage message = new SimpleMailMessage();
        message.setSubject("注册验证码");
        message.setText("注册验证码是：" + verificationCode);
        message.setFrom("alfredwatson@foxmail.com");
        message.setTo(mail);
        mailSender.send(message);
    }
}
