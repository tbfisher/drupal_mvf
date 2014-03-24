<?php

/**
 * Menu page callbacks of MVF per user module.
 */

/**
 * Form for enabling/disabling outputting units of MVF field on per user basis.
 *
 * @param array $mvf_field
 *   Fully loaded MVF field, for which per user output unit status should be
 *   changed
 */
function mvf_per_user_config_form($form, &$form_state, $mvf_field) {
  $unit_field = mvf_per_user_unit_field_load($mvf_field['field_name']);
  $form['status'] = array(
    '#type' => 'item',
    '#title' => t('Current Status'),
    '#markup' => $unit_field ? t('Enabled') : t('Disabled'),
  );

  $form['field_name'] = array(
    '#type' => 'value',
    '#value' => $mvf_field['field_name'],
  );

  $form['actions'] = array(
    '#type' => 'actions',
  );

  if ($unit_field) {
    $form['actions']['disable'] = array(
      '#type' => 'submit',
      '#value' => t('Disable'),
      '#submit' => array(
        'mvf_per_user_config_form_disable_submit',
      ),
    );
  }
  else {
    $form['actions']['enable'] = array(
      '#type' => 'submit',
      '#value' => t('Enable'),
      '#validate' => array(
        'mvf_per_user_config_form_enable_validate'
      ),
      '#submit' => array(
        'mvf_per_user_config_form_enable_submit',
      ),
    );
  }

  return $form;
}

/**
 * Validate handler for 'mvf_per_user_config_form' form.
 *
 * Make sure the name of MVF field in question is shorter than maximum allowed
 * length, and thus on its per user output units can be enabled.
 */
function mvf_per_user_config_form_enable_validate($form, &$form_state) {
  try {
    mvf_per_user_unit_field_name(mvf_per_user_field_load($form_state['values']['field_name']));
  }
  catch (FieldException $e) {
    form_error($form['field_name'], $e->getMessage());
  }
}

/**
 * Submit handler for 'mvf_per_user_config_form' form.
 *
 * Enable per user output units for a MVF field in question.
 */
function mvf_per_user_config_form_enable_submit($form, &$form_state) {
  $field = mvf_per_user_field_load($form_state['values']['field_name']);
  $unit_field = array(
    'type' => 'entityreference',
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
  );
  field_create_instance($unit_instance);

  drupal_set_message(t('Successfully enabled per user output units on %field_name field.', array(
    '%field_name' => $field['field_name'],
  )));
}

/**
 * Submit handler for 'mvf_per_user_config_form' form.
 *
 * Disable per user output units for a MVF field in question.
 */
function mvf_per_user_config_form_disable_submit($form, &$form_state) {
  $field_name = $form_state['values']['field_name'];
  $unit_field = mvf_per_user_unit_field_load($field_name);
  if ($unit_field) {
    field_delete_field($unit_field['field_name']);
    drupal_set_message(t('Successfully disabled per user output units on %field_name field.', array(
      '%field_name' => $field_name,
    )));
  }
}
