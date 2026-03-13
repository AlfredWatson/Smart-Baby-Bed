package org.ciwater.iotplatform;

import org.ciwater.iotplatform.service.UtcTimeService;
import org.ciwater.iotplatform.service.UtcTimeServiceImpl;
import org.junit.jupiter.api.Test;

import java.math.BigDecimal;

/**
 * @author AlfredWatson
 * @create 2021-12-27 16:50
 */
public class UtcTest {
    @Test
    public void test01(){
        BigDecimal decimal = new BigDecimal(1594512094.5859746);
        UtcTimeService service = new UtcTimeServiceImpl();
        System.out.println(service.getFormatTime(decimal));
    }

    @Test
    public void test02(){
        String s = "2020-07-12 08:01:35";
        UtcTimeService service = new UtcTimeServiceImpl();
        System.out.println(service.getUtcTime(s));
    }
}
