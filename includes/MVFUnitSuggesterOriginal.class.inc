<?php

/**
 * @file
 * Definition of MVFUnitSuggeserOriginal class.
 */

/**
 * Easiest implementation of suggester for output units in MVF field.
 *
 * Simply output in them in whatever units values were entered.
 */
class MVFUnitSuggesterOriginal extends MVFUnitSuggesterAbstract {

  public static function getInfo($measure, $field, $instance) {
    return array(
      'title' => t('Original Units'),
    );
  }
}
