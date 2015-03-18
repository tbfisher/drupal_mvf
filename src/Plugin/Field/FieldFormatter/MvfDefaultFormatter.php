<?php

/**
 * @file
 * Contains Drupal\mvf\Plugin\Field\FieldFormatter\MvfDefaultFormatter.
 */

namespace Drupal\mvf\Plugin\Field\FieldFormatter;

use Drupal\Core\Field\Plugin\Field\FieldFormatter\DecimalFormatter;
use Drupal\Core\Field\FieldItemListInterface;
use Drupal\Core\Form\FormStateInterface;

/**
 * Plugin implementation of the 'mvf' Formatter.
 *
 * @FieldFormatter(
 *   id = "mvf",
 *   label = @Translation("MVF field"),
 *   field_types = {
 *     "mvf",
 *   }
 * )
 */
class MvfDefaultFormatter extends DecimalFormatter {

  /**
   * {@inheritdoc}
   */
  public static function defaultSettings() {
    return array(
      'unit_format' => '0',
      'unit_convert' => '',
    ) + parent::defaultSettings();
  }

  /**
   * {@inheritdoc}
   */
  public function settingsForm(array $form, FormStateInterface $form_state) {
    $elements = parent::settingsForm($form, $form_state);

    $elements['unit_format'] = array(
      '#type' => 'select',
      '#title' => t('Unit Format'),
      '#options' => array(
        '0' => $this->t('Abbreviation'),
        '1' => $this->t('Full'),
      ),
      '#default_value' => $this->getSetting('unit_format'),
    );

    $field_settings = $this->getFieldSettings();
    $units = \Drupal::service('unitsapi.units');
    $options = $units::getUnitOptions($field_settings['quantity']);
    $elements['unit_convert'] = array(
      '#type' => 'select',
      '#title' => t('Convert to Unit'),
      '#options' => array(
        '' => $this->t('No conversion.'),
      ) + $options,
      '#default_value' => $this->getSetting('unit_convert'),
    );

    return $elements;
  }

  /**
   * {@inheritdoc}
   */
  public function viewElements(FieldItemListInterface $items) {
    $elements = array();
    $settings = $this->getFieldSettings();

    $units = \Drupal::service('unitsapi.units');
    $unit_labels = $units::getUnits($settings['quantity']);

    foreach ($items as $delta => $item) {

      // Units.
      if ($this->getSetting('unit_convert')) {
        $output = $this->numberFormat($units::convert($item->value, $settings['quantity'], $item->unit, $this->getSetting('unit_convert')));
        $label = $unit_labels[$item->unit];
      }
      else {
        $output = $this->numberFormat($item->value);
        $label = $unit_labels[$item->unit];
      }
      $label = $settings['unit_format'] ? $this->t($label[0]) : $this->formatPlural($item->value, $label[1], $label[2]);
      $output = $output . '&nbsp;<span class="unit">' . $label . '</span>';

      // Account for prefix and suffix.
      if ($this->getSetting('prefix_suffix')) {
        $prefixes = isset($settings['prefix']) ? array_map(array($this, 'fieldFilterXss'), explode('|', $settings['prefix'])) : array('');
        $suffixes = isset($settings['suffix']) ? array_map(array($this, 'fieldFilterXss'), explode('|', $settings['suffix'])) : array('');
        $prefix = (count($prefixes) > 1) ? $this->formatPlural($item->value, $prefixes[0], $prefixes[1]) : $prefixes[0];
        $suffix = (count($suffixes) > 1) ? $this->formatPlural($item->value, $suffixes[0], $suffixes[1]) : $suffixes[0];
        $output = $prefix . $output . $suffix;
      }
      // Output the raw value in a content attribute if the text of the HTML
      // element differs from the raw value (for example when a prefix is used).
      if (!empty($item->_attributes) && $item->value != $output) {
        $item->_attributes += array('content' => $item->value);
      }

      $elements[$delta] = array('#markup' => $output);
    }

    return $elements;
  }

}
