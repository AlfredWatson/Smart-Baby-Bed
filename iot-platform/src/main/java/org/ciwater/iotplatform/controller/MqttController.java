package org.ciwater.iotplatform.controller;

import org.ciwater.iotplatform.dao.MqttGateway;
import org.ciwater.iotplatform.pojo.MqttMessage;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RestController;

import javax.annotation.Resource;

/**
 * @author AlfredWatson
 * @create 2022-06-07 20:04
 */
@RestController
public class MqttController {

    @Resource
    private MqttGateway mqttGateway;

    @PostMapping("/sendMqttMessage-01")
    public String sendMqttMessage01() {
        MqttMessage mqttMessage = new MqttMessage("CiWater-ServerToSensor-48:3F:DA:9B:E8:C7","服务器消息1");
        System.out.println("发送"+ mqttMessage.toString());
        // 发送消息到指定主题
        mqttGateway.sendToMqtt(mqttMessage.getTopic(), 1, mqttMessage.getContent());
        return "send topic: " + mqttMessage.getTopic() + ", message : " + mqttMessage.getContent();
    }
    @PostMapping("/sendMqttMessage-02")
    public String sendMqttMessage02() {
        MqttMessage mqttMessage = new MqttMessage("CiWater-ServerToSensor-48:3F:DA:9B:E8:C7","服务器消息2");
        System.out.println("发送"+ mqttMessage.toString());
        mqttGateway.sendToMqtt(mqttMessage.getTopic(), 1, mqttMessage.getContent());
        return "send topic: " + mqttMessage.getTopic() + ", message : " + mqttMessage.getContent();
    }

}
