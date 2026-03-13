package org.ciwater.iotplatform.service;

import org.ciwater.iotplatform.dao.RoleMapper;
import org.ciwater.iotplatform.pojo.Role;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;

/**
 * @author AlfredWatson
 * @create 2021-12-30 13:58
 */
@Service
public class RoleServiceImpl implements RoleService{

    RoleMapper roleMapper;

    @Autowired
    public void setRoleMapper(RoleMapper roleMapper) {
        this.roleMapper = roleMapper;
    }

    @Override
    public List<Role> getAllRoles() {
        return roleMapper.getAllRoles();
    }

    @Override
    public boolean setUserRoles(int userId, int roleId) {
        return roleMapper.setUserRoles(userId, roleId);
    }
}
