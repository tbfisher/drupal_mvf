<?php

/**
 * @file
 * Contains Drupal\mvf\Plugin\Field\FieldWidget\MvfDefaultWidget.
 */

namespace Drupal\mvf\Plugin\Field\FieldWidget;

use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Field\FieldItemListInterface;
use Drupal\Core\Field\Plugin\Field\FieldWidget\NumberWidget;

/**
 * Plugin implementation of the 'mvf' widget.
 *
 * @FieldWidget(
 *   id = "mvf",
 *   label = @Translation("MVF field"),
 *   field_types = {
 *     "mvf",
 *   }
 * )
 */
class MvfDefaultWidget extends NumberWidget {

  /**
   * {@inheritdoc}
   */
  public function formElement(FieldItemListInterface $items, $delta, array $element, array &$form, FormStateInterface $form_state) {

    // Convert to fieldset.
    $value = parent::formElement($items, $delta, $element, $form, $form_state);
    $value = $value['value'];
    unset($value['#title']);
    $element['value'] = $value;
    $element['#type'] = 'fieldset';

    // Generate unit options.
    $field_settings = $this->getFieldSettings();
    $units = \Drupal::service('unitsapi.units');
    $options = $units::getUnitOptions($field_settings['quantity']);
    $enabled = array_filter($field_settings['units'][$field_settings['quantity']]);
    $options = array_intersect_key($options, $enabled);

    $element['unit'] = array(
      '#type' => 'select',
      '#title' => t('Unit'),
      '#options' => $options,
      '#default_value' => isset($items[$delta]->unit) ? $items[$delta]->unit : NULL,
    );

    return $element;
  }

}
