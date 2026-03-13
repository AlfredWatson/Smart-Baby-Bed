package org.ciwater.iotplatform;

import org.junit.jupiter.api.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.mail.SimpleMailMessage;
import org.springframework.mail.javamail.JavaMailSenderImpl;
import org.springframework.test.context.junit4.SpringRunner;

/**
 * @author AlfredWatson
 * @create 2021-12-28 22:05
 */
@SpringBootTest
public class MailTest {

    @Autowired
    JavaMailSenderImpl mailSender;

    @Test
    public void test() {
        String emailServiceCode = "1234";
        SimpleMailMessage message = new SimpleMailMessage();
        message.setSubject("注册验证码");
        message.setText("注册验证码是：" + emailServiceCode);
        message.setFrom("alfredwatson@foxmail.com");
        message.setTo("1731990910@qq.com");
        mailSender.send(message);
    }
}
