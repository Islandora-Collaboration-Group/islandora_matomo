<?php

/**
 * @file
 * Form and utilities used in islandora_matomo configurations.
 */

/**
 * Matomo Admin Settings form.
 */
function islandora_matomo_admin_settings() {
  $form['islandora_matomo_url'] = array(
    '#title' => t("Matomo API endpoint"),
    '#type' => 'textfield',
    '#size' => 60,
    '#default_value' => variable_get('islandora_matomo_url', 'http://matomo.example.com'),
    '#description' => t("Your Matomo instance's URL"),
    '#maxlength' => 255,
  );
  $form['islandora_matomo_internal_url'] = array(
    '#title' => t("Internal to this server Matomo API endpoint"),
    '#type' => 'textfield',
    '#size' => 60,
    '#default_value' => variable_get('islandora_matomo_internal_url', 'https://isle-matomo-ld'),
    '#description' => t("Your Matomo instance's URL as seen from your server. Normally the same as your first URL but can be different if running matomo in a non public facing instance like local docker."),
    '#maxlength' => 255,
  );
  $form['islandora_matomo_site_id'] = array(
    '#title' => t('Site ID'),
    '#type' => 'textfield',
    '#size' => 5,
    '#default_value' => variable_get('islandora_matomo_site_id', '1'),
    '#description' => t("The  Matomo site ID."),
    '#maxlength' => 5,
  );

  $form['islandora_matomo_timeout'] = array(
    '#title' => t('Timeout in seconds'),
    '#type' => 'textfield',
    '#size' => 2,
    '#default_value' => variable_get('islandora_matomo_timeout', '2'),
    '#description' => t("Used for direct/ not async HTTP API calls to track Datastream Views and downloads"),
    '#maxlength' => 2,
  );

  $form['islandora_matomo_moredimensions_site_id'] = array(
    '#title' => t('Site ID for extra Tracked Dimensions'),
    '#type' => 'textfield',
    '#size' => 5,
    '#default_value' => variable_get('islandora_matomo_moredimensions_site_id', '2'),
    '#description' => t("The Matomo site ID used to track Authors and Departments."),
    '#maxlength' => 5,
  );
  $form['islandora_matomo_author_solrfield'] = array(
    '#title' => t('Solr field containing a list of authors. This applies for all objects except Scholar profiles.'),
    '#type' => 'textfield',
    '#size' => 128,
    '#autocomplete_path' => 'islandora_solr/autocomplete_luke',
    '#default_value' => variable_get('islandora_matomo_author_solrfield', 'mods_identifier_u1_ms'),
    '#maxlength' => 128,
  );

  $form['islandora_matomo_department_solrfield'] = array(
    '#title' => t('Solr field containing a list of departments. This applies for all objects except Department Entities and Scholar profiles.'),
    '#type' => 'textfield',
    '#size' => 128,
    '#autocomplete_path' => 'islandora_solr/autocomplete_luke',
    '#default_value' => variable_get('islandora_matomo_department_solrfield', 'mods_identifier_u2_ms'),
    '#maxlength' => 128,
  );

  $form['islandora_matomo_source_author_solrfield'] = array(
    '#title' => t('Solr field containing a single author ID , e.g in a Scholar profile MADS record (Person CMODEL). Used to match Scholar Profiles to their Islandora Objects.'),
    '#type' => 'textfield',
    '#size' => 128,
    '#autocomplete_path' => 'islandora_solr/autocomplete_luke',
    '#default_value' => variable_get('islandora_matomo_source_author_solrfield', 'mads_u1_s'),
    '#maxlength' => 128,
  );

  $form['islandora_matomo_source_department_solrfield'] = array(
    '#title' => t('Solr field containing a single Department ID. e.g in a Department profile MADS record (Organization CMODEL). Used to match Departments to their Islandora Objects.'),
    '#type' => 'textfield',
    '#size' => 128,
    '#autocomplete_path' => 'islandora_solr/autocomplete_luke',
    '#default_value' => variable_get('islandora_matomo_source_department_solrfield', 'mads_u2_ms'),
    '#maxlength' => 128,
  );

  $form['islandora_matomo_collection_site_ids'] = array(
    '#title' => t('Collection-specific site IDs'),
    '#type' => 'textarea',
    '#size' => 5,
    '#default_value' => variable_get('islandora_matomo_collection_site_ids', ''),
    '#description' => t("Matomo site IDs for specific collections, one entry per
      row. Each row should be in PID, site ID format
      (e.g., \"islandora:mycollection, 5\"). Multiple collections can be
      associated with the same site ID."),
  );

  $form['islandora_matomo_track_collection_usage'] = array(
    '#type' => 'checkbox',
    '#title' => t('Track collection usage'),
    '#default_value' => variable_get('islandora_matomo_track_collection_usage', 1),
    '#description' => t("Check this option if you want to track collection usage
      in addtion to object usage. If enabled, each object's parent collections
      are counted every time the object is viewed, and usage reports will be
      available under \"Visitors > Custom Variables\" in your Matomo server.
      High-traffic or large sites may want to disable this option to improve
      performance."),
  );
  $form['islandora_matomo_api_key'] = array(
    '#title' => t("Matomo API key"),
    '#type' => 'textfield',
    '#size' => 40,
    '#default_value' => variable_get('islandora_matomo_api_key', ''),
    '#description' => t("Your Matomo's API key (a.k.a. token_auth). Required if you want to view reports generated by Matomo. It is available under any account with widget access on the Matomo server, in the API menu. Warning! Make sure you don't use a Full Super Admin account's token in production since that token could give full access to the API."),
    '#maxlength' => 40,
  );

  $form['islandora_matomo_widget_api_key'] = array(
    '#title' => t("Matomo API key for Widget in a Block Display"),
    '#type' => 'textfield',
    '#size' => 40,
    '#default_value' => variable_get('islandora_matomo_widget_api_key', 'anonymous'),
    '#description' => t("Your Matomo's API key (a.k.a. token_auth). Required to display embedded Matomo Widgets in each Object. It is available under any account with widget access on the Matomo server, in the API menu. Warning! Make sure you don't use a Full Super Admin account's token in production since that token could give full access to the API.If using 'anonymous' make sure to setup in matomo the 'view' privileges for that user"),
    '#maxlength' => 40,
  );
  $form['islandora_matomo_javascript_tracking_code'] = array(
    '#title' => t('Matomo JavaScript tracking code'),
    '#type' => 'textarea',
    '#size' => 5,
    '#default_value' => variable_get('islandora_matomo_javascript_tracking_code', ''),
    '#description' => t("If you want to use Matomo to provide analytics for your entire
      Islandora site, enter the \"JavaScript Tracking Code\" generated by Matomo here.
      Note that if you update any of the options provided by Matomo, you will need to
      update this JavaScript code. Islandora objects are not counted twice."),
  );
  $form['islandora_matomo_send_emails'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send automated Matomo reports to Drupal users.'),
    '#default_value' => variable_get('islandora_matomo_send_emails', 1),
    '#description' => t("Check this option if you want send automated reports via email
      to Drupal users that have a matching Islandora Scholar Profile. 
      Please make sure that the User has a matching key value setup in the Drupal User/Account admin form. 
      Opt-in/out can be set individually on each Account.)"),
  );

  return system_settings_form($form);
}
