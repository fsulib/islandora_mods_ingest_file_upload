<?php
/**
 * @file
 * Helper functions used by this module.
 */


/**
 * Populates the MODS datastream with the uploaded MODS record
 *
 * @param AbstractObject $object
 *   A Fedora object.
 * @param object $file
 *   A Drupal file object containing the MODS.
 *
 * @return bool
 *   TRUE if the MODS datastream was created FALSE if it was updated.
 */
function islandora_mods_ingest_file_upload_ingest_mods(AbstractObject $object, $file) {
  $created = FALSE;
  if (empty($object['MODS'])) {
    $ds = $object->constructDatastream('MODS', 'M');
    $object->ingestDatastream($ds);
    $created = TRUE;
  }
  else {
    $ds = $object['MODS'];
  }
  $ds->content = file_get_contents($file->uri);
  return $created;
}
