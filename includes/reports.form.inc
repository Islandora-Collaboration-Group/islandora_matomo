<?php

function islandora_matomo_report_form($form, &$form_state) {
  $index_page = rtrim(variable_get('islandora_matomo_url', 'http://matomo.example.com'),'/') .'/index.php';
  $general_site_id = variable_get('islandora_matomo_site_id', '1');
  $otherdimensions_site_id = variable_get('islandora_matomo_moredimensions_site_id', '2');
  $token_auth = variable_get('islandora_matomo_api_key', '');
  $day_ini = new DateTime("today");
  $day_end = new DateTime("yesterday");
  $diff = $day_end->diff($day_ini)->format("%a");
  $start_date =  $day_ini->format('m/d/Y'); // 2012-02-01
  $end_date = $day_end->format('m/d/Y'); // 2012-02-29
  $start_date_m = $day_ini->format('Y-m-d'); // 2012-02-01
  $end_date_m =$day_end->format('Y-m-d'); // 2012-02-01
  //
  //drupal_add_library('system', 'jquery.form');
  if (isset($form_state['values']['initial_date'])) {

    $initial_date = $form_state['values']['initial_date'];
    $end_date = $form_state['values']['end_date'];
    $date1 = new DateTime($initial_date);
    $date2 = new DateTime($end_date);

    $start_date_m = $date1->format('Y-m-d'); // 2012-02-01
    $end_date_m =$date2->format('Y-m-d'); // 2012-02-01
    $start_date =  $date1->format('m/d/Y'); // 2012-02-01
    $end_date = $date2->format('m/d/Y'); // 2012-02-29
    $diff = $date2->diff($date1)->format("%a");
  }
  $diff = $diff +1;
  $markup = <<<MARKUP
<h3>Real-time map</h3>
<div id="widgetIframe1"><iframe width="100%" height="350" src="$index_page?module=Widgetize&action=iframe&widget=1&moduleToWidgetize=UserCountryMap&actionToWidgetize=realtimeMap&idSite=$general_site_id&period=range&date=$start_date_m,$end_date_m&widget=1&token_auth=$token_auth" scrolling="yes" frameborder="0" marginheight="0" marginwidth="0"></iframe></div>

<h3>Visitor map</h3>
<div id="widgetIframe2"><iframe width="100%" height="350" src="$index_page?module=Widgetize&action=iframe&widget=1&moduleToWidgetize=UserCountryMapIslandora&actionToWidgetize=visitorMap&idSite=$general_site_id&period=range&date=$start_date_m,$end_date_m&disableLink=1&widget=1&token_auth=$token_auth" scrolling="yes" frameborder="0" marginheight="0" marginwidth="0"></iframe></div>

<h3>Web Pages</h3>
<div id="widgetIframe3"><iframe width="100%" height="350" src="$index_page?module=Widgetize&action=iframe&widget=1&moduleToWidgetize=Actions&actionToWidgetize=getPageTitles&filter_limit=50&idSite=$general_site_id&period=range&date=$start_date_m,$end_date_m&disableLink=1&widget=1&token_auth=$token_auth" scrolling="yes" frameborder="0" marginheight="0" marginwidth="0"></iframe></div>

<h3>Islandora collection usage</h3>
<div id="widgetIframe4"><iframe width="100%" height="350" src="$index_page?module=Widgetize&action=iframe&widget=1&moduleToWidgetize=CustomDimensions&actionToWidgetize=getCustomDimension&idDimension=1&idSite=$general_site_id&period=range&date=$start_date_m,$end_date_m&disableLink=1&widget=1&filter_limit=50&token_auth=$token_auth" scrolling="yes" frameborder="0" marginheight="0" marginwidth="0"></iframe></div>

<h3>Islandora Object usage</h3>
<div id="widgetIframe4"><iframe width="100%" height="350" src="$index_page?module=Widgetize&action=iframe&widget=1&moduleToWidgetize=CustomDimensions&actionToWidgetize=getCustomDimension&idDimension=2&idSite=$general_site_id&period=range&date=$start_date_m,$end_date_m&disableLink=1&widget=1&filter_limit=50&token_auth=$token_auth" scrolling="yes" frameborder="0" marginheight="0" marginwidth="0"></iframe></div>

<h3>Readership by Authors</h3>
<div id="widgetIframe4"><iframe width="100%" height="350" src="$index_page?module=Widgetize&action=iframe&widget=1&moduleToWidgetize=CustomDimensions&actionToWidgetize=getCustomDimension&idDimension=3&idSite=$otherdimensions_site_id&period=range&date=$start_date_m,$end_date_m&disableLink=1&widget=1&filter_limit=50&token_auth=$token_auth" scrolling="yes" frameborder="0" marginheight="0" marginwidth="0"></iframe></div>

<h3>Readership by Departments</h3>
<div id="widgetIframe4"><iframe width="100%" height="350" src="$index_page?module=Widgetize&action=iframe&widget=1&moduleToWidgetize=CustomDimensions&actionToWidgetize=getCustomDimension&idDimension=4&idSite=$otherdimensions_site_id&period=month&date=today&disableLink=1&widget=1&filter_limit=50&token_auth=$token_auth" scrolling="yes" frameborder="0" marginheight="0" marginwidth="0"></iframe></div>


<h3>Searches</h3>
<div id="widgetIframe5"><iframe width="100%" height="350" src="$index_page?module=Widgetize&action=iframe&widget=1&moduleToWidgetize=Actions&actionToWidgetize=getSiteSearchKeywords&idSite=$general_site_id&period=range&date=$start_date_m,$end_date_m&disableLink=1&widget=1&filter_limit=50&token_auth=$token_auth" scrolling="yes" frameborder="0" marginheight="0" marginwidth="0"></iframe></div>

<h3>Searches returning no results</h3>
<div id="widgetIframe6"><iframe width="100%" height="350" src="$index_page?module=Widgetize&action=iframe&widget=1&moduleToWidgetize=Actions&actionToWidgetize=getSiteSearchNoResultKeywords&idSite=$general_site_id&period=range&date=$start_date_m,$end_date_m&disableLink=0&widget=1&filter_limit=50&token_auth=$token_auth" scrolling="yes" frameborder="0" marginheight="0" marginwidth="0"></iframe></div>
MARKUP;

  $form['initial_date'] = [
    '#type' => 'textfield',
    '#title' => 'Start Date',
    '#attributes' => array('class' => array('datepicker')),
    '#default_value' => $start_date,
  ];
  $form['end_date'] = [
    '#type' => 'textfield',
    '#title' => 'End Date',
    '#attributes' => ['class' => array('datepicker')],
    '#default_value' => $end_date,
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

function _islandora_matomo_get_userprofile_bypid($pid) {
  $key = NULL;
  $query_processor = new IslandoraSolrQueryProcessor();
  $query = '*:*';
  $bindingfield = trim(variable_get('islandora_matomo_source_author_solrfield', 'MADS_u1_ms'));
  $query_processor->buildQuery($query);
  // Todo make this solr field configurable.
  $query_processor->solrParams['fl'] = 'PID,' . trim($bindingfield);
  $query_processor->solrParams['fq'][] = 'PID:' . "\"" . $pid ."\"";
  $query_processor->solrParams['fq'][] = 'RELS_EXT_hasModel_uri_ms:' . "\"info:fedora/islandora:personCModel\"";
  try {
    $query_processor->executeQuery(FALSE);
    $solr_results = $query_processor->islandoraSolrResult['response'];
    if ($solr_results['numFound'] > 0) {
      //@TODO recheck this, could be single value instead of an array?
      $key = $solr_results['objects'][0]['solr_doc'][$bindingfield][0];
    }
  } catch (Exception $e) {
    watchdog(
      'islandora_matomo',
      'Islandora Matomo found errors getting the Scholar profile Key for !pid with error: !error',
      [
        '!pid' => $pid,
        '!error' => check_plain($e->getMessage())

      ],
      WATCHDOG_WARNING
    );

  }
  return $key;
}

function islandora_matomo_readershipreport_form($form, &$form_state, AbstractObject $object) {
  if ($object) {
    $month_ini = new DateTime("first day of last month");
    $month_end = new DateTime("last day of last month");
    $diff = $month_end->diff($month_ini)->format("%a");
    $start_date =  $month_ini->format('m/d/Y'); // 2012-02-01
    $end_date = $month_end->format('m/d/Y'); // 2012-02-29
    $start_date_m = $month_ini->format('Y-m-d'); // 2012-02-01
    $end_date_m =$month_end->format('Y-m-d'); // 2012-02-01
    $index_page = rtrim(
        variable_get('islandora_matomo_url', 'http://matomo.example.com'),
        '/'
      ) . '/index.php';
    $otherdimensions_site_id = variable_get(
      'islandora_matomo_moredimensions_site_id',
      '2'
    );
    $token_auth = variable_get('islandora_matomo_api_key', '');
    $userkey = _islandora_matomo_get_userprofile_bypid($object->id);

    //drupal_add_library('system', 'jquery.form');
    if (isset($form_state['values']['initial_date'])) {
      $initial_date = $form_state['values']['initial_date'];
      $end_date = $form_state['values']['end_date'];
      $date1 = new DateTime($initial_date);
      $date2 = new DateTime($end_date);
      $start_date_m = $date1->format('Y-m-d'); // 2012-02-01
      $end_date_m =$date2->format('Y-m-d'); // 2012-02-01
      $start_date =  $date1->format('m/d/Y'); // 2012-02-01
      $end_date = $date2->format('m/d/Y'); // 2012-02-29
      $diff = $date2->diff($date1)->format("%a");
    }
    $diff = $diff +1;
    // Leave this for CSS/HTML discussion <div id="htmlreport"><iframe width="100%" height="350" src="$index_page?expanded=1&filter_limit=100&format=HTML&format_metrics=1&idSite=$otherdimensions_site_id&method=Actions.getPageTitles&module=API&period=range&date=$start_date_m,$end_date_m&segment=dimension3%3D%3D$userkey&token_auth=$token_auth"></iframe></div>
    //
    $markup = <<<MARKUP
<h2>Report for a $diff days period</h2>
 
<h3>By Title</h3>
<div id="widgetIframe3"><iframe width="100%" height="450" src="$index_page?module=Widgetize&action=iframe&widget=1&moduleToWidgetize=Actions&actionToWidgetize=getPageTitles&filter_limit=50&idSite=$otherdimensions_site_id&period=range&date=$start_date_m,$end_date_m&disableLink=1&widget=1&segment=dimension3%3D%3D$userkey&token_auth=$token_auth" scrolling="yes" frameborder="0" marginheight="0" marginwidth="0"></iframe></div>

<h3>Readership by Author</h3>
<div id="widgetIframe4"><iframe width="100%" height="450" src="$index_page?module=Widgetize&action=iframe&widget=1&moduleToWidgetize=CustomDimensions&actionToWidgetize=getCustomDimension&idDimension=3&idSite=$otherdimensions_site_id&period=range&date=$start_date_m,$end_date_m&disableLink=1&widget=1&filter_limit=50&segment=dimension3%3D%3D$userkey&token_auth=$token_auth" scrolling="yes" frameborder="0" marginheight="0" marginwidth="0"></iframe></div>

<h3>Real-time map for this Scholar</h3>
<div id="widgetIframe1"><iframe width="100%" height="380" src="$index_page?module=Widgetize&action=iframe&widget=1&moduleToWidgetize=UserCountryMap&actionToWidgetize=realtimeMap&idSite=$otherdimensions_site_id&period=range&date=$start_date_m,$end_date_m&disableLink=1&widget=1&segment=dimension3%3D%3D$userkey&token_auth=$token_auth" scrolling="yes" frameborder="0" marginheight="0" marginwidth="0"></iframe></div>

<h3>Visitor map</h3>
<div id="widgetIframe2"><iframe width="100%" height="380" src="$index_page?module=Widgetize&action=iframe&widget=1&moduleToWidgetize=UserCountryMapIslandora&actionToWidgetize=visitorMap&idSite=$otherdimensions_site_id&period=range&date=$start_date_m,$end_date_m&disableLink=1&widget=1&segment=dimension3%3D%3D$userkey&token_auth=$token_auth" scrolling="yes" frameborder="0" marginheight="0" marginwidth="0"></iframe></div>

MARKUP;

    $form['initial_date'] = [
      '#type' => 'textfield',
      '#title' => 'Start Date',
      '#attributes' => ['class' => ['datepicker']],
      '#default_value' => $start_date,
    ];
    $form['end_date'] = [
      '#type' => 'textfield',
      '#title' => 'End Date',
      '#attributes' => ['class' => ['datepicker']],
      '#default_value' => $end_date,
    ];
    // Same submit as the previous one.
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
      $form['#after_build'] = ['islandora_matomo_report_form_uidatepicker'];
    }

    return $form;
  }
  return $form;
}

function islandora_matomo_readershipreport_form_submit(array $form, array &$form_state) {

  $form_state['rebuild'] = TRUE;
  return $form;

}