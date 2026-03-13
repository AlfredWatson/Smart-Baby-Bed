package org.ciwater.iotplatform.pojo;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

/**
 * @author AlfredWatson
 * @create 2022-06-07 20:02
 */
@Data
@NoArgsConstructor
@AllArgsConstructor
public class MqttMessage {
    private String topic;
    private String content;
}