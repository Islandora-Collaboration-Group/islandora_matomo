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

  if (user_access('view islandora matomo reports')) {
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
