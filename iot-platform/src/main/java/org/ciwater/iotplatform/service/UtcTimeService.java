package org.ciwater.iotplatform.service;

import java.math.BigDecimal;

/**
 * @author AlfredWatson
 * @create 2021-12-27 16:45
 */
public interface UtcTimeService {
    /**
     * 将UTC时间转换为yyyy-MM-dd HH:mm:ss形式的标准时间
     * @param utcTime: UTC时间
     * @return yyyy-MM-dd HH:mm:ss形式的标准时间
     */
    String getFormatTime(BigDecimal utcTime);

    /**
     * 将yyyy-MM-dd HH:mm:ss形式的标准时间转换为UTC时间
     * @param formatTime: yyyy-MM-dd HH:mm:ss形式的标准时间
     * @return UTC时间
     */
    BigDecimal getUtcTime(String formatTime);
}
