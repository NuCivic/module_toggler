<?php

/**
 * Page callback for admin/modules/toggle
 */
function module_toggler_list_page() {
  return theme('module_toggler_list', array('module_groups' => module_toggler_get_module_groups()));
}

function module_toggler_enable_confirm() {
  $form = array();
  $question = t('Do you want to enable?');
  $path = 'admin/modules/toggle';
  $yes = t('Confirm');
  $no = t('Cancel');

  return confirm_form($question, $path, $yes, $no);
}
