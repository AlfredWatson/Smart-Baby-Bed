package org.ciwater.iotplatform.dao;

import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;
import org.ciwater.iotplatform.pojo.User;
import org.springframework.stereotype.Repository;

/**
 * @author AlfredWatson
 * @create 2021-12-29 15:35
 */
@Mapper
@Repository
public interface UserMapper {
    User getUserByName(@Param("userName") String name);

    /**
     * 增加一个新用户
     * @param name
     * @param password
     * @param mail
     * @param enable
     * @return 是否增加成功
     */
    boolean setUser(@Param("userName") String name,
                    @Param("userPassword") String password,
                    @Param("userMail") String mail,
                    @Param("userEnable") boolean enable);
}
