<?php

/**
 * @file
 * Definition of MVFPerUserUnitSuggesterUser class.
 */

/**
 * Implementation of suggester for output units in MVF field.
 *
 * Suggest a unit, which currently logged in user has specified in his/her
 * profile settings.
 */
class MVFPerUserUnitSuggesterUser extends MVFUnitSuggesterAbstract {

  public static function getInfo($measure, $field, $instance) {
    return array(
      'title' => t('Per User'),
    );
  }

  public static function enabled($measure, $field, $instance, $view_mode, $settings = array()) {
    // Making sure corresponding unit field is created.
    if (!mvf_per_user_unit_field_load($field['field_name'])) {
      try {
        $unit_field = array(
          'type' => $field['settings']['meta_info']['unit']['field_type'],
          'field_name' => mvf_per_user_unit_field_name($field),
          // We allow to edit this field via Field UI because folks may want to
          // customize lots of things about it.
          'locked' => FALSE,
          // We will copy settings for our entityreference field from corresponding
          // part of the MVF field.
          'settings' => $field['settings']['unit'],
        );

        $unit_field = field_create_field($unit_field);
        $unit_instance = array(
          'field_name' => $unit_field['field_name'],
          'entity_type' => 'user',
          'bundle' => 'user',
          'label' => t('Output Unit for @field_name', array(
            '@field_name' => $field['field_name'],
          )),
          'description' => t('Please, specify here in what units you want to see %field_name field.', array(
            '%field_name' => $field['field_name'],
          )),
          'widget' => array(
            'type' => $field['settings']['meta_info']['unit']['widget'],
          ),
        );
        field_create_instance($unit_instance);
      }
      catch (FieldException $e) {
        drupal_set_message($e->getMessage(), 'error');
      }
    }
  }

  public static function disabled($measure, $field, $instance, $view_mode, $settings = array()) {
    // Removing corresponding unit field if this is the last MVF field, where
    // per user UnitSuggester was enabled.
    $field_map = field_info_field_map();
    $field_info = $field_map[$field['field_name']];
    foreach ($field_info['bundles'] as $entity_type => $bundles) {
      foreach ($bundles as $bundle) {
        $instance = field_info_instance($entity_type, $field['field_name'], $bundle);
        foreach ($instance['display'] as $instance_display) {
          $instance_settings = mvf_unit_suggester_settings_extract($instance_display, __CLASS__);
          if ($instance_settings['enable']) {
            // At least for one display per user output is still enabled, so
            // we should not delete the corresponding unit field.
            return;
          }
        }
      }
    }

    // If we got down here, it means not a single display for this MVF field has
    // per user output enabled. So we will remove the corresponding unit field.
    $unit_field = mvf_per_user_unit_field_load($field['field_name']);
    field_delete_field($unit_field['field_name']);
  }

  public static function getSettingsForm($measure, $field, $instance, $settings = array()) {
    $form = array();

    $form['info'] = array(
      '#markup' => t('Users will be able to specify in what units to output this field in their profile settings, if you enable it here.'),
    );

    return $form;
  }

  public static function suggestUnit($items, $field, $instance, $entity, $entity_type, $settings = array()) {
    if ($GLOBALS['user']->uid == 0) {
      return MVF_UNIT_UNKNOWN;
    }
    // Fully loading the user.
    $account = user_load($GLOBALS['user']->uid);
    $items = field_get_items('user', $account, mvf_per_user_unit_field_name($field));
    if (isset($items[0][mvf_subfield_to_column('unit')]) && $items[0][mvf_subfield_to_column('unit')]) {
      // If there is specified desired output, we suggest that output unit.
      return $items[0][mvf_subfield_to_column('unit')];
    }
    return MVF_UNIT_UNKNOWN;
  }

}
