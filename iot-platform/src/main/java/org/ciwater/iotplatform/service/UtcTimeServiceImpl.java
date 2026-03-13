package org.ciwater.iotplatform.service;

import java.math.BigDecimal;
import java.text.DecimalFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * @author AlfredWatson
 * @create 2021-12-27 16:47
 */
public class UtcTimeServiceImpl implements UtcTimeService {
    @Override
    public String getFormatTime(BigDecimal utcTime) {
        SimpleDateFormat simpleDateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        // 四舍五入 仅保留整数 并转换为long类型
        long a = new Long(utcTime.setScale(0, BigDecimal.ROUND_HALF_UP).toString());
        // 乘1000是秒;不乘是毫秒
        Date date = new Date(a * 1000);
        return simpleDateFormat.format(date);
    }

    @Override
    public BigDecimal getUtcTime(String formatTime) {
        SimpleDateFormat simpleDateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        try {
            Date date = simpleDateFormat.parse(formatTime);
            long a = date.getTime();
            return new BigDecimal(String.valueOf(a / 1000));
        } catch (ParseException e) {
            e.printStackTrace();
        }
        return null;
    }

}
