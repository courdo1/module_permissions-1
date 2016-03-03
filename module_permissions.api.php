<?php

/**
 * @file
 * Hooks provided by the module_permissions API.
 */

/**
 * Allows modules to deny or provide access for a user to modules permissions.
 *
 * Modules implementing this hook can return FALSE to provide a blanket
 * prevention for the user to perform the requested operation on the specified
 * page. If no modules implementing this hook return FALSE but at least one
 * returns TRUE, then the operation will be allowed, even for a user without
 * role based permission to perform the operation.
 *
 * If no modules return FALSE but none return TRUE either, normal permission
 * based checking will apply.
 *
 * @param $op - managed_module_list or managed_module_permissions.
 * @param $account - The user account whose access should be determined.
 */
function hook_module_permissions_access($op, $account) {
  // Placeholder.
}
