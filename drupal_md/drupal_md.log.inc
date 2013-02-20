<?php

/**
 * @file
 * Show log messages
 */

function drupal_md_log($offset = 0, $wid = 0) {
  $account = drupal_md_get_user_by_session('view log on mobile');
  if ((string)$offset == 'event') {
    print_single_event($wid);
    exit;
  }
  $offset = 0; //should be removed later, with range call added to below query
  $rows = array();
  $classes = array(
    WATCHDOG_DEBUG     => 'dblog-debug',
    WATCHDOG_INFO      => 'dblog-info',
    WATCHDOG_NOTICE    => 'dblog-notice',
    WATCHDOG_WARNING   => 'dblog-warning',
    WATCHDOG_ERROR     => 'dblog-error',
    WATCHDOG_CRITICAL  => 'dblog-critical',
    WATCHDOG_ALERT     => 'dblog-alert',
    WATCHDOG_EMERGENCY => 'dblog-emerg',
  );
  $header = array(
    '', // Icon column.
    array('data' => t('Type'), 'field' => 'w.type'),
    array('data' => t('Date'), 'field' => 'w.wid', 'sort' => 'desc'),
    t('Message'),
    array('data' => t('User'), 'field' => 'u.name'),
    array('data' => t('Operations')),
  );

  $query = db_select('watchdog', 'w')->extend('PagerDefault')->extend('TableSort');
  $query->leftJoin('users', 'u', 'w.uid = u.uid');
  $query
    ->fields('w', array('wid', 'uid', 'severity', 'type', 'timestamp', 'message', 'variables', 'link'))
    ->addField('u', 'name');

  $result = $query
    ->orderByHeader($header)    
    ->limit(20)
    ->execute();
  $events = array();
  foreach ($result as $dblog) {
    $event = array();
    $event['wid'] = intval($dblog->wid);
    $event['uid'] = intval($dblog->uid);
    $event['severity'] = $dblog->severity;
    $event['type'] = $dblog->type;
    $event['timestamp'] = intval($dblog->timestamp);
    $msg = strtr($dblog->message, unserialize($dblog->variables));
    if ($msg) {
      $event['message'] = $msg;
    } else {
      $event['message'] = $event['type'];
    }
    array_push($events, $event);
  }
  drupal_json_output(array('message' => 'ok', 'events' => $events, 'offset' => $offset));
  exit;
}

function print_single_event($wid) {
  $severity = watchdog_severity_levels();
  $result = db_query(
    'SELECT w.*, u.name as uname, u.uid FROM {watchdog} w '
    .'INNER JOIN {users} u ON w.uid = u.uid '
    .'WHERE w.wid = :id',
    array(':id' => $wid))->fetchObject();
  if ($dblog = $result) {
    $event = array();
    $event['wid'] = intval($dblog->wid);
    $event['uid'] = intval($dblog->uid);
    $event['name'] = strval($dblog->uname);
    $event['severity'] = $dblog->severity;
    $event['type'] = $dblog->type;
    $event['timestamp'] = intval($dblog->timestamp);
    $msg = strtr($dblog->message, unserialize($dblog->variables));
    if ($msg) {
      $event['message'] = $msg;
    } else {
      $event['message'] = $event['type'];
    }
    $event['hostname'] = $dblog->hostname;
    $event['location'] = $dblog->location;
    $event['referer'] = $dblog->referer;
    drupal_json_output(array('message' => 'ok', 'event' => $event));
    exit;
  } else {
    drupal_md_not_found();
    exit;
  }
}