<?php
/**
 * @file
 * orphan_object.admin.inc
 *
 */

 /**
 * Menu callback for the Orphan Object Administration settings form.
 */
function orphan_object_admin_settings() {
  module_load_include('inc', 'fedora_repository', 'api/fedora_utils');
  $collectionsquery = 'select $object $title from <#ri>
                            where ($object <dc:title> $title
                              and $object <info:fedora/fedora-system:def/model#hasModel> <info:fedora/islandora:collectionCModel>)
                            order by $title';
  $collectionsurl = variable_get('fedora_repository_url', 'http://localhost:8080/fedora/risearch');
  $collectionsurl .= "?type=tuples&flush=TRUE&format=csv&limit=1000&lang=itql&stream=on&query=";
  $collectionscontent = do_curl($collectionsurl . htmlentities(urlencode($collectionsquery)));
  $collectionslist = explode("\n", $collectionscontent);
  array_shift($collectionslist);
  $collectionslist = preg_replace('/info:fedora\//', '', $collectionslist);
  $collections = array();
  foreach ($collectionslist as $collectionsitem) {   //removes  blanks
    if (!empty($collectionsitem) && $collectionsitem != 'islandora:root,Islandora Top-level Collection') {
      array_push($collections, $collectionsitem);
    }
  }
  $select_options = array();
  foreach ($collections as $item) {
    $select_options[$item] = $item;
  }
  $form['orphanobject_collection_default'] = array(
    '#type' => 'select',
    '#title' => t('Default collection used by Orphan Object Administration'),
    '#default_value' => variable_get('orphanobject_collection_default', 'islandora:sp_generic_collection,Islandora GENERIC Collection'),
    '#options' => $select_options,
    '#description' => t('Select the Collection Orphan Object Administration uses to find Orphans.'),
  );
  return system_settings_form($form);
}
