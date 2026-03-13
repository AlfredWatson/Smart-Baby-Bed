package org.ciwater.iotplatform.pojo;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.math.BigDecimal;

/**
 * @author AlfredWatson
 * @create 2022-06-07 20:29
 */
@Data
@NoArgsConstructor
@AllArgsConstructor
public class SensorArrayDataSub {
    private String utcTime;
    private int deviceId;
    private BigDecimal sensorHumidity;
    private BigDecimal sensorTemp;
}
