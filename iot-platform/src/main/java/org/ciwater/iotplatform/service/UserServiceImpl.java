package org.ciwater.iotplatform.service;

import org.ciwater.iotplatform.dao.RoleMapper;
import org.ciwater.iotplatform.dao.UserMapper;
import org.ciwater.iotplatform.pojo.User;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.security.core.userdetails.UserDetailsService;
import org.springframework.security.core.userdetails.UsernameNotFoundException;
import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.stereotype.Service;

/**
 * @author AlfredWatson
 * @create 2021-12-29 16:13
 */
@Service("userService")
public class UserServiceImpl implements UserService {

    private UserMapper userMapper;
//    private RoleMapper roleMapper;

    @Autowired
    public void setUserMapper(UserMapper userMapper) {
        this.userMapper = userMapper;
    }

//    @Autowired
//    public void setRoleMapper(RoleMapper roleMapper) {
//        this.roleMapper = roleMapper;
//    }

    // UserService接口实现
    @Override
    public User getUserByName(String name) {
        return userMapper.getUserByName(name);
    }

    @Override
    public boolean setUser(String name, String password, String mail, boolean enable) {
        PasswordEncoder passwordEncoder = new BCryptPasswordEncoder();
        String pwd = passwordEncoder.encode(password);
        return userMapper.setUser(name, pwd, mail, enable);
    }

    // UserDetailsService接口实现
    @Override
    public UserDetails loadUserByUsername(String username) throws UsernameNotFoundException {
        User user = getUserByName(username);
//        System.out.println("service:loadUserByUsername-1-" + user);
        if (user == null) {
            throw new UsernameNotFoundException("用户名不存在!");
        }
//        user.setRoleList(roleMapper.getAllRoles());
//        System.out.println("service:loadUserByUsername-2-" + user);
        // 因为数据库中的密码是明文，所以这里要加密
        PasswordEncoder passwordEncoder = new BCryptPasswordEncoder();
        String pa = passwordEncoder.encode(user.getUserPassword());
        user.setUserPassword(pa);
        System.out.println("加密之后：" + pa);
        return user;
    }
}
