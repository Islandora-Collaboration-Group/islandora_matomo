<?PHP

/**
 * @file
 * Holds block definitions for the islandora matomo module.
 */


/**
* Constructs the matomo widget block for islandora objects.
*
* @param AbstractObject $object
*   If supplied the object to create the block for.
*
* @return array
*   Renderable array for the block.
*/
function islandora_matomo_widgetonobject_block(AbstractObject $object = NULL) {
  // drupal_add_css(drupal_get_path('module', 'islandora_matomo') . '/css/islandora_matomo.block.css');
  dpm('calling the block');
  $object = $object ? $object : menu_get_object('islandora_object', 2);
  if ($object) {
    $output = '';
    if ($object) {
     $pid = $object->id;
      // This object is part of a compound object.
      $widget = theme('islandora_matomo_widget', array('PID' => $pid));
      $output .= $widget;
      if (!empty($output)) {
        return array('#markup' => $output);
      }
    }
  }
}