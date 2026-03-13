package org.ciwater.iotplatform.pojo;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.math.BigDecimal;

/**
 * @author AlfredWatson
 * @create 2021-12-26 18:23
 */
@Data
@NoArgsConstructor
@AllArgsConstructor
public class SensorArrayData {
    private BigDecimal utcTime;
    private int deviceId;
    private BigDecimal sensorCO;
    private BigDecimal sensorHumidity;
    private String sensorLight;
    private BigDecimal sensorLPG;
    private String sensorMotion;
    private BigDecimal sensorSmoke;
    private BigDecimal sensorTemp;
}
