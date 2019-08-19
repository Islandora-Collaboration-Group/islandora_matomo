<?php

/**
 * @file
 * Theme/theme.inc.
 *
 * Theme functions.
 */

/**
 * Theme function for the matomo widget block.
 */
function template_preprocess_islandora_matomo_widget(array &$variables) {

  $variables['matomo_site'] = '';

  $matomo_url = rtrim(
      variable_get('islandora_matomo_url', 'http://matomo.example.com'),
      '/'
    ) . '/';
  $variables['matomo_url'] = $endpoint = $matomo_url . 'index.php';
  $variables['matomo_site'] = $siteId = variable_get(
    'islandora_matomo_site_id',
    '1'
  );

  if (user_access(ISLANDORA_MATOMO_ISLANDORA_OBJECT_REPORT_VIEW_PERMISSION)) {
    $variables['matomo_token'] = $token_auth = variable_get(
      'islandora_matomo_api_key',
      NULL
    );
  }
  else {
    $variables['matomo_token'] = variable_get('islandora_matomo_widget_api_key', 'anonymous');
  }

  $link = '?module=Widgetize&action=iframe';
  $link .= '&disableLink=0&widget=1';

  $link2 = $link . '&moduleToWidgetize=UserCountryMap&actionToWidgetize=realtimeMap&idSite=' . $variables['matomo_site'];
  $link2 .= '&period=month&date=today&disableLink=1&widget=1&removeOldVisits=0';
  // @TODO allow config to set which widget will be displayed, the islandora or the main one.
  $link .= '&moduleToWidgetize=UserCountryMapIslandora&actionToWidgetize=visitorMap&idSite=' . $variables['matomo_site'];
  $link .= '&period=year&date=today&disableLink=1&widget=1';

  $object = islandora_object_load($variables['PID']);
  if ($object && !in_array('islandora:collectionCModel', $object->models)) {
    $link .= '&segment=' . urlencode(urlencode('dimension2==' . $variables['PID']));
    $link2 .= '&segment=' . urlencode(urlencode('dimension2==' . $variables['PID']));
  }
  else {
    $link .= '&segment=' . urlencode(urlencode('dimension1==' . $variables['PID']));
    $link2 .= '&segment=' . urlencode(urlencode('dimension1==' . $variables['PID']));
  }

  $link .= '&token_auth=' . $variables['matomo_token'];
  $link2 .= '&token_auth=' . $variables['matomo_token'];

  $variables['matomo_iframe_url'] = $variables['matomo_url'] . $link;
  $variables['matomo_iframe_url2'] = $variables['matomo_url'] . $link2;
}

/**
 * Theme function for the matomo count block.
 */
function template_preprocess_islandora_matomo_count(array &$variables) {
  // Add custom CSS.
  drupal_add_css( drupal_get_path('module', 'islandora_matomo').  '/css/matomo_blocks.css', array(
    'preprocess' => FALSE,
  ));

  $variables['matomo_site'] = '';

  $matomo_url = rtrim(
      variable_get('islandora_matomo_url', 'http://matomo.example.com'),
      '/'
    ) . '/';
  if (user_access(ISLANDORA_MATOMO_ISLANDORA_OBJECT_COUNT_VIEW_PERMISSION)) {
    $variables['matomo_token'] = $token_auth = variable_get(
      'islandora_matomo_api_key',
      NULL
    );
  }
  else {
    $variables['matomo_token'] = variable_get('islandora_matomo_widget_api_key', 'anonymous');
  }
  $variables['matomo_url'] = $endpoint = $matomo_url . 'index.php';
  $variables['matomo_site'] = $siteId = variable_get(
    'islandora_matomo_site_id',
    '1'
  );
  // This is not set by default but can be overriden by a preprocess hook if needed.
  $start_date = isset($variables['start_date']) ? $variables['start_date'] : '2019-01-01';
  $enddate_time = new DateTime();
  $enddate = $enddate_time->format('Y-m-d');

  $values = _islandora_matomo_fetch_visits_remotejson($variables['PID'], $start_date, $enddate);
  $variables['stats'] = $values;
}


/**
 * Fetches remote JSON data, visitis and downloads from matomo for a single DO.
 *
 * @param string $objectpid
 * @param string $start_date
 * @param string $end_date
 *
 * @return mixed
 */
function _islandora_matomo_fetch_visits_remotejson($objectpid, $start_date = '2019-01-01', $end_date) {
  $data = [];
  $data2 = [];
  $values['visits']  = 0;
  $values['dowloads']  = 0;

  $objectpid = urlencode($objectpid);

  $index_page = rtrim(
      variable_get('islandora_matomo_internal_url', 'https://isle-matomo-ld'),
      '/'
    ) . '/index.php';
  $dimensions_site_id = variable_get(
    'islandora_matomo_site_id',
    '1'
  );
  $token_auth = variable_get('islandora_matomo_api_key', '');
  // Visits URL
  $url = "$index_page?apiAction=getCustomDimension&apiModule=CustomDimensions&date=$start_date,$end_date&filter_limit=1&flat=1&format=JSON&format_metrics=1&idDimension=2&idSite=$dimensions_site_id&segment=dimension2%253D%253D$objectpid&method=API.getProcessedReport&module=API&reportUniqueId=CustomDimensions_getCustomDimension_idDimension--2&token_auth=$token_auth";
  $response = drupal_http_request($url);
  if (!empty($response->error)) {
    watchdog(
      'islandora_matomo',
      'Islandora Matomo found errors getting the JSON Object Usage Stats for Object !objectpid with error: Code @code, %error for URL %url',
      [
        '!objectpid' => urldecode($objectpid),
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
  // Downloads are reported as nb_visits. If no download response will be an empty JSON
  $url_downloads = "$index_page?method=Actions.getDownloads&module=API&date=$start_date,$end_date&period=range&filter_limit=1&flat=1&format=JSON&format_metrics=1&idDimension=2&idSite=$dimensions_site_id&segment=dimension2%253D%253D$objectpid&reportUniqueId=CustomDimensions_getCustomDimension_idDimension--2&token_auth=$token_auth";

   $response2 = drupal_http_request($url_downloads);
  if (!empty($response->error)) {
    watchdog(
      'islandora_matomo',
      'Islandora Matomo found errors getting the JSON Object Download Stats for Object !objectpid with error: Code @code, %error for URL %url',
      [
        '!objectpid' => urldecode($objectpid),
        '@code' => $response->code,
        '%error' => $response->error,
        '%url' => $url_downloads,

      ],
      WATCHDOG_WARNING
    );

  }
  else {
    $data2 = drupal_json_decode($response2->data);
  }



  if (isset($data['reportTotal'])) {
    $values['visits'] = isset($data['reportTotal']['nb_visits']) ? $data['reportTotal']['nb_visits'] : 0;
  }
  if (isset($data2['reportTotal'])) {
    $values['downloads'] = isset($data2['reportTotal']['nb_hits']) ? $data2['reportTotal']['nb_hits'] : 0;
  }

  return $values;
}
