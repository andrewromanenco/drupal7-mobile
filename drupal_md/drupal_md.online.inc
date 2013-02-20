<?php

/**
 * @file
 * Show online users
 */

function drupal_md_online($offset = 0) {
  $account = drupal_md_get_user_by_session('view online users on mobile');
  $interval = REQUEST_TIME - variable_get('user_block_seconds_online', 900);
  $authenticated_count = db_query(
    "SELECT COUNT(DISTINCT s.uid) FROM {sessions} s WHERE s.timestamp >= :timestamp AND s.uid > 0",
    array(':timestamp' => $interval)
    )->fetchField();

  $items = array();
  if ($authenticated_count > 0) {
    $start = intval($offset);
    $end = $start + 20;
    $items = db_query_range('SELECT u.uid, u.name, MAX(s.timestamp) AS max_timestamp '
        .'FROM {users} u INNER JOIN {sessions} s ON u.uid = s.uid '
        .'WHERE s.timestamp >= :interval AND s.uid > 0 '
        .'GROUP BY u.uid, u.name ORDER BY max_timestamp DESC',
        $start, $end, array(':interval' => $interval))->fetchAll();
    drupal_json_output(array('message' => 'ok', 'users' => $items,
      'authenticated' => intval($authenticated_count), 'offset' => intval($start)));
    exit;
  }
  drupal_json_output(array('message' => 'ok', 'total' => 0));
  exit;
}