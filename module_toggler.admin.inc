<?php

/**
 * Page callback for admin/modules/toggle
 */
function module_toggler_list_page() {
  drupal_static_reset('module_toggler_get_module_groups');
  return theme('module_toggler_list', array('module_groups' => module_toggler_get_module_groups()));
}

/**
 * Form function for module toggle confirm. Submit function does the actual
 * toggling
 *
 * @param $group
 *   The module group the enabling module belongs to
 *
 * @param $module
 *   The module to enable
 *
 * @param $op
 *   Whether we are enabling or disabling the module. A string set to either
 *   "enable" or "disable".
 *
 * @return array
 */
function module_toggler_confirm($form, &$form_state, $group, $module, $op = 'enable') {
  // Confirm that we have a valid module and group name.
  if (!module_toggler_check_group_module($group, $module)) {
    drupal_not_found();
    exit;
  }

  // Since we didn't exit after the last conditional check, we can assume that
  // the group and module do exist. Now check the status.
  $groups = module_toggler_get_module_groups();
  $module_config = $groups[$group]['modules'][$module];

  // Reject based on op and status
  if (($op == 'enable' && $module_config['status'])
    || ($op == 'disable' && !$module_config['status'])) {
    drupal_not_found();
    exit();
  }

  // If we've made it to this point, the group exists, the module is defined,
  // and the module is disabled. It can be safely enabled.
  $form = array(
    'module' => array(
      '#type' => 'hidden',
      '#value' => $module,
    ),
    'group' => array(
      '#type' => 'hidden',
      '#value' => $group,
    ),
    'toggle_op' => array(
      '#type' => 'hidden',
      '#value' => $op,
    ),
  );
  $op_name = t($op);
  $question = t('Are you sure you want to @op the %module module?', array(
    '%module' => $module_config['name'],
    '@op' => $op_name
  ));
  $path = variable_get('module_toggler_path', 'admin/modules/toggle');
  $description = t('Description');
  $yes = t(ucwords($op));
  $no = t('Cancel');

  return confirm_form($form, $question, $path, $description, $yes, $no);
}

/**
 * Validate function for enabling a module. Provide one more check to make sure
 * no one is enabling a module they shouldn't
 */
function module_toggler_confirm_validate($form, &$form_state) {
  $group = $form_state['values']['group'];
  $module = $form_state['values']['module'];
  $op = $form_state['values']['toggle_op'];

  $error_string_values = array(
    '@op' => t($op),
  );

  if (module_toggler_check_group_module($group, $module)) {
    $groups = module_toggler_get_module_groups();
    $module_config = $groups[$group]['modules'][$module];

    // Reject based on op and status
    if (($op == 'enable' && $module_config['status'])
      || ($op == 'disable' && !$module_config['status'])) {
      form_error($form['module'], t('You tried to @op an invalid module.', $error_string_values));
    }
  }
  else {
    form_error($form['module'], t('You tried to @op an invalid module.', $error_string_values));
  }
}

/**
 * Submit handler for module_toggler_confirm form.
 */
function module_toggler_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $module = $form_state['values']['module'];
    $group = $form_state['values']['group'];
    $op = $form_state['values']['toggle_op'];
    $op_name = t($op . 'd');
    $groups = module_toggler_get_module_groups();
    $module_config = $groups[$group]['modules'][$module];

    switch ($op) {
      case 'enable':
        module_enable(array($module), TRUE);
        break;

      case 'disable':
        module_disable(array($module), TRUE);
        break;
    }

    drupal_set_message(t('The %module module has been @op.', array('%module' => $module_config['name'], '@op' => $op_name)));
  }
  $form_state['redirect'] = 'admin/modules/toggle';
}

/**
 * System settings for for Module Toggler
 */
function module_toggler_settings($form, &$form_state) {
  $form = array();

  $form['module_toggler_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Base path'),
    '#default_value' => variable_get('module_toggler_path', 'admin/modules/toggle'),
    '#description' => t('Path for the main Module Toggler page and the base for sub-paths.'),
    '#required' => TRUE,
  );

  $form['module_toggler_page_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Module Toggler page title'),
    '#default_value' => variable_get('module_toggler_page_title', t('Module Toggler')),
    '#description' => t('Custom title for main Module Toggler page.'),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}

function module_toggler_settings_submit($form, &$form) {
  menu_cache_clear_all();
}