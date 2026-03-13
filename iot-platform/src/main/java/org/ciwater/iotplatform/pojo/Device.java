package org.ciwater.iotplatform.pojo;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;
import org.springframework.context.annotation.Bean;

import java.math.BigDecimal;

/**
 * @author AlfredWatson
 * @create 2021-12-26 18:19
 */
@Data
@NoArgsConstructor
@AllArgsConstructor
public class Device {
    private int deviceId;
    private String deviceMac;
}
