<?php

/**
 * @file
 * View site status
 */

function drupal_md_status() {
  drupal_md_get_user_by_session('view status on mobile');
  include_once DRUPAL_ROOT . '/includes/install.inc';
  drupal_load_updates();

  // Check run-time requirements and status information.
  $requirements = module_invoke_all('requirements', 'runtime');
  usort($requirements, '_system_sort_requirements');


  //string tags
  $cleaned = array();
  foreach ($requirements as $item) {
      $item['value'] = strip_tags($item['value']);
      if (isset($item['description'])) {
        $item['description'] = strip_tags($item['description']);
      }
      array_push($cleaned, $item);
  }

  drupal_json_output(array('message' => 'ok', 'items' => $cleaned));
  exit;
}
