<?php

/**
 * Returns HTML for the Module Toggler list page.
 *
 * @param $variables
 *   An associative array containing:
 *   - module_groups: An associative array containing groups of modules; should
 *     be the result of a module_toggler_proccess_modules() function call.
 *
 * @ingroup themeable
 */
function theme_module_toggler_list($variables) {
  $groups = $variables['module_groups'];

  $output = '<div id="module-toggler-list">';
  foreach ($groups as $group => $group_config) {
    // Start new module group.
    $output .= '<section class="module-toggler-list-'. $group .' clearfix">';
    $output .= '<header><h2>' . t($group_config['name']) .'</h2>';
    $output .= t($group_config['description']) . '</header>';

    foreach($group_config['modules'] as $module => $module_config) {
      // Start a new module
      $enabled = $module_config['status'] ? 'enabled' : 'disabled';
      $output .= '<article class="module-toggler-module .' . $module . ' module-toggler-module- .' . $enabled . '">';

      // Theme the icon.
      $output .= isset($module_config['icon'])
        ? '<figure class="icon">' . theme('image', $module_config['icon']) . '</figure>'
        : '<figure class="no-icon">' . t('no icon') . '</figure>';

      // Module name and description
      $output .= '<header><h3>' . $module_config['name'] . '</h3></header>';
      $output .= '<p>' . $module_config['description'] . '</p>';

      // More info link
      if (isset($module_config['more_info'])) {
        $output .= l(t('More information'), $module_config['more_info'], array('attributes' => array('class' => array('more-info'))));
      }

      // "Action" links
      $output .= theme('module_toggler_module_links', array(
        'group' => $group,
        'module' => $module,
        'module_config' => $module_config));
    }
  }
  return $output;
}

/**
 * Create links for a specific module
 *
 * @param $variables
 *   An associative array containing:
 *   - module: The machine of the specific module
 *   - module_config: A config array for the module
 *
 * @ingroup themeable
 */
function theme_module_toggler_module_links($variables) {
  $group = $variables['group'];
  $module = $variables['module'];
  $module_config = $variables['module_config'];
  $links = array();
  $module_t_params = array('%module' => $module_config['name']);

  if ($module_config['status']) {
    $links = array();
    $links[] = array(
      'title' => t('Disable'),
      'href' => "admin/modules/toggle/$group/$module/disable/confirm",
      'attributes' => array('title' => t('Disable %module', $module_t_params)),
    );
    if (isset($module_config['configure'])) {
      $links[] = array(
        'title' => t('Configure'),
        'href' => $module_config['configure'],
        'attributes' => array('title' => t('Configure %module', $module_t_params)),
      );
    }
  }
  else {
    $links = array(
      array(
        'title' => t('Enable'),
        'href' => "admin/modules/toggle/$group/$module/enable/confirm",
        'attributes' => array('title' => t('Enable %module', $module_t_params)),
      )
    );
  }
  return theme('links', array('links' => $links));
}
