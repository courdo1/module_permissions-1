<?php

/**
 * @file
 * Module Permissions admin paths.
 */

/**
 * Menu callback; provides managed module enable/disable interface.
 *
 * @ingroup forms
 * @see system_modules()
 */
function module_permissions_managed_modules($form, $form_state = array()) {
  // Get current list of modules.
  $files = system_rebuild_module_data();

  // Remove hidden modules from display list.
  $visible_files = $files;
  foreach ($visible_files as $filename => $file) {
    if (!empty($file->info['hidden'])) {
      unset($visible_files[$filename]);
    }
  }

  uasort($visible_files, 'module_permissions_sort_modules_by_info_name');

  // Remove module permissions from list.
  unset($visible_files['module_permissions']);

  // Create module listing.
  $managed_modules = module_permissions_managed_modules_load();
  $form['modules'] = array('#tree' => TRUE);

  // Iterate through each of the modules and add to form.
  foreach ($visible_files as $filename => $module) {
    $form['modules'][$module->info['package']][$filename] = array(
      '#tree' => TRUE,
      'granted' => array(
        '#type' => 'checkbox',
        '#title' => t('Granted'),
        '#default_value' => isset($managed_modules[$filename]) ? 1 : 0,
      ),
      'name' => array(
        '#markup' => $module->info['name'],
      ),
      'version' => array(
        '#markup' => $module->info['version'],
      ),
      'description' => array(
        '#markup' => t('!description', array('!description' => $module->info['description'])),
      ),
    );
  }

  // Add basic information to the fieldsets.
  foreach (element_children($form['modules']) as $package) {
    $form['modules'][$package] += array(
      '#type' => 'fieldset',
      '#title' => t('!package', array('!package' => $package)),
      '#collapsible' => TRUE,
      '#theme' => 'module_permissions_managed_modules_list',
      '#header' => array(
        array(
          'data' => t('Granted'),
          'class' => array('checkbox'),
        ),
        t('Name'),
        t('Version'),
        t('Description'),
      ),
      // Ensure that the "Core" package fieldset comes first.
      '#weight' => $package == 'Core' ? -10 : NULL,
    );
  }

  // Lastly, sort all fieldsets by title.
  uasort($form['modules'], 'element_sort_by_title');

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  $form['#submit'][] = 'module_permissions_managed_modules_submit';

  return $form;
}

/**
 * Form submission handler for module_permissions_managed_modules().
 */
function module_permissions_managed_modules_submit($form, &$form_state) {
  $managed_modules = array();
  foreach ($form_state['values']['modules'] as $modules) {
    foreach ($modules as $name => $values) {
      $modules = $values['granted'];
      if ($modules) {
        $managed_modules[] = $name;
      }
    }
  }
  variable_set('module_permissions_managed_modules', $managed_modules);
  drupal_set_message(t('The configuration options have been saved.'));
}

/**
 * Managed modules list theme.
 */
function theme_module_permissions_managed_modules_list($variables) {
  $form = $variables['form'];

  // Individual table headers.
  $rows = array();
  // Iterate through all the modules, which are children of this fieldset.
  foreach (element_children($form) as $key) {
    // Stick it into $module for easier accessing.
    $module = $form[$key];
    $row = array();
    unset($module['granted']['#title']);
    $row[] = array(
      'class' => array('checkbox'),
      'data' => drupal_render($module['granted'])
    );
    $label = '<label';
    if (isset($module['modules']['#id'])) {
      $label .= ' for="' . $module['granted']['#id'] . '"';
    }
    $row[] = $label . '><strong>' . drupal_render($module['name']) . '</strong></label>';
    $row[] = drupal_render($module['version']);
    $description = drupal_render($module['description']);
    $row[] = array('data' => $description, 'class' => array('description'));
    $rows[] = $row;
  }

  return theme('table', array('header' => $form['#header'], 'rows' => $rows));
}

/**
 * Array sorting callback; sorts modules by their name.
 */
function module_permissions_sort_modules_by_info_name($a, $b) {
  return strcasecmp($a->info['name'], $b->info['name']);
}
