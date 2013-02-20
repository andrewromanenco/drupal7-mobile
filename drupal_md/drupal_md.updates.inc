<?php

/**
 * @file
 * Show available updates
 */

function drupal_md_updates() {
  //'view drupal updates on mobile'
  $last_check = variable_get('update_last_check', 0);
  module_load_include('module', 'update');
  module_load_include('inc', 'update', 'update.report');
  module_load_include('module', 'update');
  $available = update_get_available(true);
  $items = array();
  foreach ($available as $item) {
    if ($item['title'] <> '')
      array_push($items, $item['title']);
  }
  drupal_json_output(array('message' => 'ok', 'last_check' => $last_check, 'updates' => $items));
  
  exit;
}
