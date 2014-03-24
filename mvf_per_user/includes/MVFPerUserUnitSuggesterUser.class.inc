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

  public static function getSettingsForm($measure, $field, $instance, $settings = array()) {
    $form = array();

    $form['status'] = array(
      '#type' => 'item',
      '#title' => t('Current Status'),
      '#markup' => mvf_per_user_unit_field_load($field['field_name']) ? t('Enabled') : t('Disabled'),
    );

    $form['info'] = array(
      '#markup' => t('You can adjust settings of per user output unit following <a href="!url">this link</a>.', array(
        '!url' => url('mvf-per-user/' . $field['field_name']),
      )),
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
