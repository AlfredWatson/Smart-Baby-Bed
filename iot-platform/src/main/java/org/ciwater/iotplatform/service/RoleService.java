package org.ciwater.iotplatform.service;

import org.ciwater.iotplatform.pojo.Role;
import org.springframework.stereotype.Service;

import java.util.List;

/**
 * @author AlfredWatson
 * @create 2021-12-30 13:58
 */
@Service
public interface RoleService {
    List<Role> getAllRoles();

    boolean setUserRoles(int userId, int roleId);
}
