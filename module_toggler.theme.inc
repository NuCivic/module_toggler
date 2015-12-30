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
    $output .= '<section class="panel panel-default module-toggler-list-'. $group .' clearfix">';
    $output .= '<header class="panel-heading"><h2>' . t($group_config['name']) .'</h2>';
    if (isset($group_config['description'])) {
      $output .= t($group_config['description']);
    }
    $output .= '</header>';

    foreach($group_config['modules'] as $module => $module_config) {
      // Start a new module
      switch ($module_config['status']) {
        case 1:
          $enabled = 'enabled';
          $status_label = 'success';
          $status_title = t('This component is enabled on your website.');
          break;
        case 0:
          $enabled = 'disabled';
          $status_label = 'default';
          $status_title = t('This component is disabled but available on your website.');
          break;
        case -1:
          $enabled = 'unavailable';
          $status_label = 'danger';
          $status_title = t('This component is not available on your website. Contact your system administrator for help.');
          break;
      }
      $output .= '<article class="module-toggler-module .' . $module . ' module-toggler-module- .' . $enabled . '">';

      // Theme the icon.
      $output .= isset($module_config['icon'])
        ? '<figure class="icon">' . theme('image', $module_config['icon']) . '</figure>'
        : '<figure class="no-icon">' . t('no icon') . '</figure>';

      // Module name and description
      $output .= '<header><h3>' . $module_config['name'] . '</h3></header>';

      // Status label
      $output .= '<ul class="module-metadata">';
      $output .= '<li class="module-status status-' . $enabled .'">';
      $output .= '<span title="' . $status_title . '" class="label label-' . $status_label . '">';
      $output .= $enabled . '</span></li></ul>';

      $output .= '<p>' . $module_config['description'] . '</p>';

      // "Action" links
      $output .= theme('module_toggler_module_links', array(
        'group' => $group,
        'module' => $module,
        'module_config' => $module_config));

      $output .= '</article>';
    }
    $output .= '</section>';
  }
  $output .= '</div>';
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
  $module_t_params = array('%module' => $module_config['name']);

  $output = '<div class="btn-group">';

  switch ($module_config['status']) {
    case 1:
      $output .= l(t('Disable'), "admin/modules/toggle/$group/$module/disable/confirm", array(
        'attributes' => array(
          'title' => t('Disable %module', $module_t_params),
          'class' => array('btn', 'btn-primary'),
        ),
      ));

      if (isset($module_config['configure'])) {
        $output .= l(t('Configure'), $module_config['configure'], array(
          'attributes' => array(
            'title' => t('Configure %module', $module_t_params),
            'class' => array('btn', 'btn-default'),
          ),
        ));
      }
      break;

    case 0:
      $output .= l(t('Enable'), "admin/modules/toggle/$group/$module/enable/confirm", array(
        'attributes' => array(
          'title' => t('Enable %module', $module_t_params),
          'class' => array('btn', 'btn-primary'),
        ),
      ));
      break;

    case -1:
      $output .= '<span class="btn btn-danger">' . t('Enable') . '</span>';
      break;
  }

  // More info link
  if (isset($module_config['more_info'])) {
    $output .= l(t('Information'), $module_config['more_info'], array(
      'attributes' => array(
        'title' => t('More information on %module', $module_t_params),
        'class' => array('btn', 'btn-default'),
        'target' => '_blank',
      ),
    ));
  }


  $output .= '</div>';
  return $output;
}
