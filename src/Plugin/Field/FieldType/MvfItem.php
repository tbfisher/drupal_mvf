<?php

/**
 * @file
 * Contains Drupal\mvf\Plugin\Field\FieldType\MvfItem.
 */

namespace Drupal\mvf\Plugin\Field\FieldType;

use Drupal\Core\Field\FieldStorageDefinitionInterface;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\TypedData\DataDefinition;
use Drupal\Core\Field\Plugin\Field\FieldType\FloatItem;

/**
 * Defines the 'mvf' field type.
 *
 * @FieldType(
 *   id = "mvf",
 *   label = @Translation("Number (float) with units"),
 *   description = @Translation("This field stores a number in the database in a floating point format with units."),
 *   default_widget = "mvf",
 *   default_formatter = "mvf"
 * )
 */
class MvfItem extends FloatItem {

  /**
   * {@inheritdoc}
   */
  public static function propertyDefinitions(FieldStorageDefinitionInterface $field_definition) {
    $properties = parent::propertyDefinitions($field_definition);
    $properties['unit'] = DataDefinition::create('string')
      ->setLabel(t('Unit'))
      ->setRequired(TRUE);
    return $properties;
  }

  /**
   * {@inheritdoc}
   */
  public static function schema(FieldStorageDefinitionInterface $field_definition) {
    $schema = parent::schema($field_definition);
    $schema['columns']['unit'] = array(
      'type' => 'varchar',
      'length' => 255,
    );
    return $schema;
  }

  /**
   * {@inheritdoc}
   */
  public static function defaultStorageSettings() {
    return array(
      'quantity' => 'Mass',
    ) + parent::defaultStorageSettings();
  }

  /**
   * {@inheritdoc}
   */
  public function storageSettingsForm(array &$form, FormStateInterface $form_state, $has_data) {
    $element = parent::storageSettingsForm($form, $form_state, $has_data);

    $settings = $this->getSettings();
    $units = \Drupal::service('unitsapi.units');

    $element['quantity'] = array(
      '#name' => 'quantity',
      '#type' => $has_data ? 'value' : 'select',
      '#title' => t('Quantity'),
      '#options' => $units::getQuantities(),
      '#default_value' => $settings['quantity'],
    );

    return $element;
  }

  /**
   * {@inheritdoc}
   */
  public static function defaultFieldSettings() {
    return array(
      'units' => array(),
    ) + parent::defaultFieldSettings();
  }

  /**
   * {@inheritdoc}
   */
  public function fieldSettingsForm(array $form, FormStateInterface $form_state) {
    $element = parent::fieldSettingsForm($form, $form_state);
    $settings = $this->getSettings();
    $units = \Drupal::service('unitsapi.units');

    $quantities = $units::getQuantities();
    $element['units'] = array(
      '#type' => 'checkboxes',
      '#title' => $quantities[$settings['quantity']] . ' ' . t('units'),
      '#options' => $units::getUnitOptions($settings['quantity']),
      '#default_value' => $settings['units'],
    );

    return $element;
  }

}
