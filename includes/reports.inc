<?php

/**
 * Page callback that dumbly renders Piwik widget iframe markup.
 */

function islandora_matomo_reports() {
  $index_page = rtrim(variable_get('islandora_matomo_url', 'http://matomo.example.com'),'/') .'/index.php';
  $general_site_id = variable_get('islandora_matomo_site_id', '1');
  $token_auth = variable_get('islandora_matomo_api_key', '');

  $markup = <<<MARKUP
<div>Using data collected today.</div>
<h3>Real-time map</h3>
<div id="widgetIframe1"><iframe width="100%" height="350" src="$index_page?module=Widgetize&action=iframe&widget=1&moduleToWidgetize=UserCountryMap&actionToWidgetize=realtimeMap&idSite=$general_site_id&period=day&date=today&disableLink=1&widget=1&token_auth=$token_auth" scrolling="yes" frameborder="0" marginheight="0" marginwidth="0"></iframe></div>

<h3>Visitor map</h3>
<div id="widgetIframe2"><iframe width="100%" height="350" src="$index_page?module=Widgetize&action=iframe&widget=1&moduleToWidgetize=UserCountryMap&actionToWidgetize=visitorMap&idSite=$general_site_id&period=day&date=today&disableLink=1&widget=1&token_auth=$token_auth" scrolling="yes" frameborder="0" marginheight="0" marginwidth="0"></iframe></div>

<h3>Site pages</h3>
<div id="widgetIframe"3><iframe width="100%" height="350" src="$index_page?module=Widgetize&action=iframe&widget=1&moduleToWidgetize=Actions&actionToWidgetize=getPageUrls&idSite=$general_site_id&period=day&date=today&disableLink=1&widget=1&token_auth=$token_auth" scrolling="yes" frameborder="0" marginheight="0" marginwidth="0"></iframe></div>

<h3>Islandora collection usage</h3>
<div id="widgetIframe4"><iframe width="100%" height="350" src="$index_page?module=Widgetize&action=iframe&widget=1&moduleToWidgetize=CustomDimensions&actionToWidgetize=getCustomDimension&idDimension=1&idSite=$general_site_id&period=day&date=today&disableLink=1&widget=1&token_auth=$token_auth" scrolling="yes" frameborder="0" marginheight="0" marginwidth="0"></iframe></div>

<h3>Searches</h3>
<div id="widgetIframe5"><iframe width="100%" height="350" src="$index_page?module=Widgetize&action=iframe&widget=1&moduleToWidgetize=Actions&actionToWidgetize=getSiteSearchKeywords&idSite=$general_site_id&period=day&date=today&disableLink=1&widget=1&token_auth=$token_auth" scrolling="yes" frameborder="0" marginheight="0" marginwidth="0"></iframe></div>

<h3>Searches returning no results</h3>
<div id="widgetIframe6"><iframe width="100%" height="350" src="$index_page?module=Widgetize&action=iframe&widget=1&moduleToWidgetize=Actions&actionToWidgetize=getSiteSearchNoResultKeywords&idSite=$general_site_id&period=day&date=today&disableLink=1&widget=1&token_auth=$token_auth" scrolling="yes" frameborder="0" marginheight="0" marginwidth="0"></iframe></div>
MARKUP;
  return $markup;
}