<?php

/**
 * @file
 * Deals with sending reports via email.
 */

/**
 * Performs any matomo notifications that should be done from cron.
 *
 * @see \islandora_matomo_mail()
 */
function islandora_matomo_user_notify($user, $pid, $userkey, $objectcount = 0) {
  $field = 'field_islandoramatomo_sendreport';
  $module = 'islandora_matomo';
  $to = $user->mail;
  $from = variable_get('site_mail', 'admin@example.com');
  $language = language_default();
  $send = TRUE;

  $params = array();
  $params['user'] = $user;
  $params['profile_pid'] = $pid;

  $month_ini = new DateTime("first day of last month");
  $month_end = new DateTime("last day of last month");

  // 2012-02-01.
  $params['start_date'] = $month_ini->format('Y-m-d');
  // 2012-02-01.
  $params['end_date'] = $month_end->format('Y-m-d');
  $params['month'] = $month_ini->format('F');
  $params['year'] = $month_ini->format('Y');
  $params['objectcount'] = $objectcount;

  $data = islandora_matomo_fetch_remotejson($userkey, $params['start_date'], $params['end_date']);
  if (isset($data['reportTotal'])) {
    $params['data'] = $data;
    $params['hits'] = isset($data['reportTotal']['nb_hits']) ? $data['reportTotal']['nb_hits'] : 0;
  }
  if (!empty($params) && !empty($params['data'])) {
    $message = drupal_mail('islandora_matomo', 'status_notify', $to, $language, $params, $from, $send);
    // Track when the last mail was successfully sent to avoid sending
    // too many e-mails.
    if ($message['result']) {
      $edit[$field][LANGUAGE_NONE][0]['value'] = REQUEST_TIME;
      user_save($user,$edit);
    }
  }
}

/**
 * Gets the object count for a user using the userkey.
 */
function islandora_matomo_get_objectcount_foruser($userkey) {

  $num_objects = 0;
  $query_processor = new IslandoraSolrQueryProcessor();
  $query = '*:*';
  $query_processor->buildQuery($query);
  // Todo make this solr field configurable.
  $query_processor->solrParams['fl'] = 'PID,' . trim(variable_get('islandora_matomo_author_solrfield', 'mods_identifier_u1_ms'));
  $query_processor->solrParams['fq'][] = trim(variable_get('islandora_matomo_author_solrfield', 'mods_identifier_u1_ms')) . ':' . "{$userkey}";
  try {
    $query_processor->executeQuery(FALSE);
    $solr_results = $query_processor->islandoraSolrResult['response'];
    if ($solr_results['numFound'] > 0) {
      $num_objects = $solr_results['numFound'];
    }
  }
  catch (Exception $e) {
    watchdog('islandora_matomo', 'Islandora Matomo found errors getting Object count for user !userkey with error: !error',
      array(
        '!userkey' => $userkey,
        '!error' => check_plain($e->getMessage()),

      ), WATCHDOG_WARNING);

  }
  return $num_objects;
}

/**
 * Gets the userprofile based on the unique userkey.
 */
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
      $pid = $solr_results['objects'][0]['solr_doc']['PID'];
    }
  }
  catch (Exception $e) {
    watchdog(
      'islandora_matomo',
      'Islandora Matomo found errors getting the Scholar profile for user !userkey with error: !error',
      [
        '!userkey' => $userkey,
        '!error' => check_plain($e->getMessage()),

      ],
      WATCHDOG_WARNING
      );

  }
  return $pid;
}

/**
 * Fetches remote JSON data from matomo.
 */
function islandora_matomo_fetch_remotejson($userkey, $start_date, $end_date) {
  $data = [];
  $index_page = rtrim(
      variable_get('islandora_matomo_internal_url', 'https://isle-matomo-ld'),
      '/'
    ) . '/index.php';
  $otherdimensions_site_id = variable_get(
    'islandora_matomo_moredimensions_site_id',
    '2'
  );
  $token_auth = variable_get('islandora_matomo_api_key', '');

  $url = "$index_page?apiAction=getCustomDimension&apiModule=CustomDimensions&date=$start_date,$end_date&filter_limit=30&flat=1&format=JSON&format_metrics=1&idDimension=3&idSite=$otherdimensions_site_id&segment=dimension3%3D%3D$userkey&method=API.getProcessedReport&module=API&period=month&reportUniqueId=CustomDimensions_getCustomDimension_idDimension--3&token_auth=$token_auth";
  $response = drupal_http_request($url);
  if (!empty($response->error)) {
    watchdog(
      'islandora_matomo',
      'Islandora Matomo found errors getting the JSON Scholar Stats for user !userkey with error: Code @code, %error for URL %url',
      [
        '!userkey' => $userkey,
        '@code' => $response->code,
        '%error' => $response->error,
        '%url' => $url,

      ],
      WATCHDOG_WARNING
    );

  }
  else {
    $data = drupal_json_decode($response->data);
  }
  return $data;
}
