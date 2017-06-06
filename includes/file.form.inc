<?php

/**
 * @file
 * Upload MODS file.
 */

/**
 * Upload MODS form.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 *
 * @return array
 *   A Drupal form definition.
 */
function islandora_mods_ingest_file_upload_file_form(array $form, array &$form_state) {
  return array(
    'file' => array(
      '#type' => 'managed_file',
      '#title' => t('MODS File'),
      '#description' => t('A file containing a MODS record. Click "Next" to skip this step and create a record from scratch.'),
      '#upload_validators' => array(
        'file_validate_extensions' => array('xml', 'mods'),
      ),
    ),
  );
}

/**
 * Populate the MODS datastream with the uploaded file.
 *
 * Only populates the MODS datastream if a file was uploaded.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 */
function islandora_mods_ingest_file_upload_file_form_submit(array &$form, array &$form_state) {
  module_load_include('inc', 'islandora_mods_ingest_file_upload', 'includes/utilities');
  $file = file_load($form_state['values']['file']);
  if ($file) {
    $mods_step_storage = &islandora_ingest_form_get_step_storage($form_state, 'islandora_mods_ingest_file_upload_upload');
    $object = islandora_ingest_form_get_object($form_state);
    $mods_step_storage['created']['MODS'] = islandora_mods_ingest_file_upload_ingest_mods($object, $file);
    file_delete($file);
    unset($form_state['values']['file']);
  }
}

/**
 * Undo's any changes the regular submit handler did.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 */
function islandora_mods_ingest_file_upload_file_form_undo_submit(array &$form, array &$form_state) {
  $mods_step_storage = &islandora_ingest_form_get_step_storage($form_state, 'islandora_mods_ingest_file_upload_upload');
  $object = islandora_ingest_form_get_object($form_state);
  if (isset($mods_step_storage['created']['MODS']) && $mods_step_storage['created']['MODS']) {
    $object->purgeDatastream('MODS');
  }
  unset($mods_step_storage['created']);
}
