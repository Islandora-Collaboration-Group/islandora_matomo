<?php


/*
 **
 * Performs any matomo notifications that should be done from cron.
 *
 * @see \islandora_matomo_mail()
*/
function islandora_matomo_user_notify($user,$pid) {
  $field = 'field_islandoramatomo_sendreport';
  $module = 'islandora_matomo';
  $to = $user->mail;
  $from = variable_get('site_mail', 'admin@example.com');
  $language = language_default();
  $send = TRUE;

  $params = array();
  $params['user'] = $user;
  $params['profile_pid'] = $pid;

  if (!empty($params)) {
    $message = drupal_mail('islandora_matomo', 'status_notify', $to, $language, $params, $from, $send);
    // Track when the last mail was successfully sent to avoid sending
    // too many e-mails.
    if ($message['result']) {
      $user->{$field}[LANGUAGE_NONE][0]['value'] = REQUEST_TIME;
    }
  }
}

function islandora_matomo_get_objectcount_foruser($userkey) {

  $num_objects = 0;
  $query_processor = new IslandoraSolrQueryProcessor();
  $query = '*:*';
  $query_processor->buildQuery($query);
  // Todo make this solr field configurable.
  $query_processor->solrParams['fl'] = 'PID,'.trim(variable_get('islandora_matomo_author_solrfield','mods_identifier_u1_ms'));
  $query_processor->solrParams['fq'][] = trim(variable_get('islandora_matomo_author_solrfield','mods_identifier_u1_ms')).':' . "{$userkey}";
  try {
    $query_processor->executeQuery(FALSE);
    $solr_results = $query_processor->islandoraSolrResult['response'];
    if ($solr_results['numFound'] > 0) {
      $num_objects = $solr_results['numFound'];
    }
  } catch (Exception $e) {
    watchdog('islandora_matomo', 'Islandora Matomo found errors getting Object count for user !userkey with error: !error',
      array(
        '!userkey' => $userkey,
        '!error' => check_plain($e->getMessage())

      ), WATCHDOG_WARNING);

  }
  return $num_objects;
}

function islandora_matomo_get_userprofile($userkey) {
  $pid = NULL;
  $query_processor = new IslandoraSolrQueryProcessor();
  $query = '*:*';
  $query_processor->buildQuery($query);
  // Todo make this solr field configurable.
  $query_processor->solrParams['fl'] = 'PID,' . trim(
      variable_get('islandora_matomo_source_author_solrfield', 'MADS_u1_ms')
    );
  $query_processor->solrParams['fq'][] = trim(
      variable_get('islandora_matomo_source_author_solrfield', 'MADS_u1_ms')
    ) . ':' . "{$userkey}";
  $query_processor->solrParams['fq'][] = 'RELS_EXT_hasModel_uri_ms:' . "\"info:fedora/islandora:personCModel\"";
  try {
    $query_processor->executeQuery(FALSE);
    $solr_results = $query_processor->islandoraSolrResult['response'];
    if ($solr_results['numFound'] > 0) {
      $PID = $solr_results['objects'][0]['solr_doc']['PID'];
    }
  } catch (Exception $e) {
    watchdog(
      'islandora_matomo',
      'Islandora Matomo found errors getting the Scholar profile for user !userkey with error: !error',
      [
        '!userkey' => $userkey,
        '!error' => check_plain($e->getMessage())

      ],
      WATCHDOG_WARNING
    );

  }
  return $pid;
}