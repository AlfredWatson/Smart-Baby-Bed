package org.ciwater.iotplatform.pojo;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

/**
 * @author AlfredWatson
 * @create 2021-12-29 16:50
 */
@Data
@NoArgsConstructor
@AllArgsConstructor
public class Role {
    private int roleId;
    private String roleName;
    private String roleDesc;
}
