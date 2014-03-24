<?php

/**
 * @file
 * Definition of MVFUnitSuggesterAbstract class.
 */

/**
 * Abstract implementation of suggester for output units in MVF field.
 *
 * A reasonable starter-class. Implements most of the methods with the most
 * general code, so extending it may result in less code written versus
 * implementing MVFUnitSuggestable interface in your class.
 */
abstract class MVFUnitSuggesterAbstract implements MVFUnitSuggestable {

  /**
   * The most straightforward implementation of the method.
   *
   * Simply return an empty settings form.
   */
  public static function getSettingsForm($measure, $field, $instance, $settings = array()) {
    return array();
  }

  /**
   * The most straightforward implementation of the method.
   *
   * Simply suggest outputting in original units.
   */
  public static function suggestUnit($items, $field, $instance, $entity, $entity_type, $settings = array()) {
    return MVF_UNIT_ORIGINAL;
  }
}
