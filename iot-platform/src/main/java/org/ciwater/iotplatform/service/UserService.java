package org.ciwater.iotplatform.service;

import org.apache.ibatis.annotations.Param;
import org.ciwater.iotplatform.pojo.User;
import org.springframework.security.core.userdetails.UserDetailsService;
import org.springframework.stereotype.Service;

/**
 * @author AlfredWatson
 * @create 2021-12-29 16:12
 */
@Service
public interface UserService extends UserDetailsService {
    User getUserByName(String name);

    boolean setUser(String name, String password, String mail, boolean enable);
}
