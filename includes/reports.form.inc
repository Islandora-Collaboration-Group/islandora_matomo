<?php

/**
 * Page callback that dumbly renders Piwik widget iframe markup.
 */

function islandora_matomo_report_form($form, &$form_state) {
  $index_page = rtrim(variable_get('islandora_matomo_url', 'http://matomo.example.com'),'/') .'/index.php';
  $general_site_id = variable_get('islandora_matomo_site_id', '1');
  $otherdimensions_site_id = variable_get('islandora_matomo_moredimensions_site_id', '2');
  $token_auth = variable_get('islandora_matomo_api_key', '');

  //
  //drupal_add_library('system', 'jquery.form');
  if (isset($form_state['values']['initial_date'])) {

    $initial_date = $form_state['values']['initial_date'];
    $end_date = $form_state['values']['end_date'];
    $date1 = new DateTime($initial_date);
    $date2 = new DateTime($end_date);

    $diff = $date2->diff($date1)->format("%a");
  }


  $markup = <<<MARKUP
<h3>Real-time map</h3>
<div id="widgetIframe1"><iframe width="100%" height="350" src="$index_page?module=Widgetize&action=iframe&widget=1&moduleToWidgetize=UserCountryMap&actionToWidgetize=realtimeMap&idSite=$general_site_id&period=week&date=today&disableLink=1&widget=1&token_auth=$token_auth" scrolling="yes" frameborder="0" marginheight="0" marginwidth="0"></iframe></div>

<h3>Visitor map</h3>
<div id="widgetIframe2"><iframe width="100%" height="350" src="$index_page?module=Widgetize&action=iframe&widget=1&moduleToWidgetize=UserCountryMap&actionToWidgetize=visitorMap&idSite=$general_site_id&period=week&date=today&disableLink=1&widget=1&token_auth=$token_auth" scrolling="yes" frameborder="0" marginheight="0" marginwidth="0"></iframe></div>

<h3>Web Pages</h3>
<div id="widgetIframe3"><iframe width="100%" height="350" src="$index_page?module=Widgetize&action=iframe&widget=1&moduleToWidgetize=Actions&actionToWidgetize=getPageTitles&filter_limit=50&idSite=$general_site_id&period=week&date=today&disableLink=1&widget=1&token_auth=$token_auth" scrolling="yes" frameborder="0" marginheight="0" marginwidth="0"></iframe></div>

<h3>Islandora collection usage</h3>
<div id="widgetIframe4"><iframe width="100%" height="350" src="$index_page?module=Widgetize&action=iframe&widget=1&moduleToWidgetize=CustomDimensions&actionToWidgetize=getCustomDimension&idDimension=1&idSite=$general_site_id&period=week&date=today&disableLink=1&widget=1&filter_limit=50&token_auth=$token_auth" scrolling="yes" frameborder="0" marginheight="0" marginwidth="0"></iframe></div>

<h3>Islandora Object usage</h3>
<div id="widgetIframe4"><iframe width="100%" height="350" src="$index_page?module=Widgetize&action=iframe&widget=1&moduleToWidgetize=CustomDimensions&actionToWidgetize=getCustomDimension&idDimension=2&idSite=$general_site_id&period=month&date=today&disableLink=1&widget=1&filter_limit=50&token_auth=$token_auth" scrolling="yes" frameborder="0" marginheight="0" marginwidth="0"></iframe></div>

<h3>Readership by Authors</h3>
<div id="widgetIframe4"><iframe width="100%" height="350" src="$index_page?module=Widgetize&action=iframe&widget=1&moduleToWidgetize=CustomDimensions&actionToWidgetize=getCustomDimension&idDimension=3&idSite=$otherdimensions_site_id&period=month&date=today&disableLink=1&widget=1&filter_limit=50&token_auth=$token_auth" scrolling="yes" frameborder="0" marginheight="0" marginwidth="0"></iframe></div>

<h3>Readership by Departments</h3>
<div id="widgetIframe4"><iframe width="100%" height="350" src="$index_page?module=Widgetize&action=iframe&widget=1&moduleToWidgetize=CustomDimensions&actionToWidgetize=getCustomDimension&idDimension=4&idSite=$otherdimensions_site_id&period=month&date=today&disableLink=1&widget=1&filter_limit=50&token_auth=$token_auth" scrolling="yes" frameborder="0" marginheight="0" marginwidth="0"></iframe></div>


<h3>Searches</h3>
<div id="widgetIframe5"><iframe width="100%" height="350" src="$index_page?module=Widgetize&action=iframe&widget=1&moduleToWidgetize=Actions&actionToWidgetize=getSiteSearchKeywords&idSite=$general_site_id&period=month&date=today&disableLink=1&widget=1&filter_limit=50&token_auth=$token_auth" scrolling="yes" frameborder="0" marginheight="0" marginwidth="0"></iframe></div>

<h3>Searches returning no results</h3>
<div id="widgetIframe6"><iframe width="100%" height="350" src="$index_page?module=Widgetize&action=iframe&widget=1&moduleToWidgetize=Actions&actionToWidgetize=getSiteSearchNoResultKeywords&idSite=$general_site_id&period=month&date=today&disableLink=0&widget=1&filter_limit=50&token_auth=$token_auth" scrolling="yes" frameborder="0" marginheight="0" marginwidth="0"></iframe></div>
MARKUP;

  $form['initial_date'] = [
    '#type' => 'textfield',
    '#title' => 'Start Date',
    '#attributes' => array('class' => array('datepicker')),
  ];
  $form['end_date'] = [
    '#type' => 'textfield',
    '#title' => 'End Date',
    '#attributes' => ['class' => array('datepicker')],
  ];
  $form['submit'] = [
    '#type' => 'submit',
    '#value' => t('Filter'),
    '#ajax' => [
      'callback' => 'islandora_matomo_report_form_ajax_updatewidgets',
      'wrapper' => 'islandora-matomo-report',
      ],
    ];
  $form['report'] = [
    '#prefix' => '<div id="islandora-matomo-report">',
    '#suffix' => '</div>',
    '#type' => 'markup',
    '#markup' => $markup
  ];
  if (!$form_state['rebuild']) {
    error_log('hu, i need this');
    $form['#after_build'] = ['islandora_matomo_report_form_uidatepicker'];
  }

  return $form;
}

function islandora_matomo_report_form_uidatepicker($form, $form_state) {
  drupal_add_library('system', 'ui.datepicker');
  drupal_add_js("(function ($) { $('.datepicker').datepicker(); })(jQuery);", array('type' => 'inline', 'scope' => 'footer', 'weight' => 5));
  return $form;
}

function islandora_matomo_report_form_submit(array $form, array &$form_state) {

  $form_state['rebuild'] = TRUE;
  return $form;

}

function islandora_matomo_report_form_validate(array $form, array &$form_state) {

  $initial_date = $form_state['values']['initial_date'];
  $end_date = $form_state['values']['end_date'];
  $date1 = new DateTime($initial_date);
  $date2 = new DateTime($end_date);
  $date_now = new DateTime();
  if ($diff = $date_now->diff($date2)->format("%a") > 0) {
    form_set_error(
      'end_date',
      t('You can not select an End Date into the future')
    );
  }
  if ($diff = $date_now->diff($date1)->format("%a") < 0) {
    form_set_error(
      'start_date',
      t('You can not select an Start Date into the future')
    );
  }

  if ($diff = $date1->diff($date2)->format("%a") < 0) {
    form_set_error('start_date', t('Start Date happens after End Date'));
  }
  if ($diff = $date1->diff($date2)->format("%a") == 0) {
    form_set_error('start_date', t('Start and End Date are the same'));
  }
}
/**
 *  Callback for ajax driven Report form.
 */
function islandora_matomo_report_form_ajax_updatewidgets($form, &$form_state) {
  return $form['report'];
}