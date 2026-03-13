package org.ciwater.iotplatform.dao;

import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;
import org.ciwater.iotplatform.pojo.Role;
import org.springframework.stereotype.Repository;

import java.util.List;

/**
 * @author AlfredWatson
 * @create 2021-12-29 16:53
 */
@Mapper
@Repository
public interface RoleMapper {
    List<Role> getAllRoles();

    boolean setUserRoles(@Param("userId") int userId, @Param("roleId") int roleId);
}
