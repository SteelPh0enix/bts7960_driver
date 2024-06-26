/// @file mock.c
/// BTS7960 HAL mock for testing and playing with.
/// This mock functions as an fully-configurable software simulation of BTS7960 HAL.

#include "mock.h"

bool BTS7960_HAL_initializeHardware(BTS7960_HAL *const hal) {
  if (hal->should_init_succeed) {
    hal->should_deinit_succeed                        = true;
    hal->should_set_enable_pin_state_succeed          = true;
    hal->should_get_enable_pin_state_succeed          = true;
    hal->should_measure_current_sense_voltage_succeed = true;
    hal->should_set_pwm_signal_percentage_succeed     = true;
    hal->should_get_pwm_signal_percentage_succeed     = true;
#ifdef BTS7960_ENABLE_FREQUENCY_CONTROL
    hal->min_allowed_frequency                   = BTS7960_HAL_MOCK_DEFAULT_MIN_ALLOWED_FREQUENCY;
    hal->max_allowed_frequency                   = BTS7960_HAL_MOCK_DEFAULT_MAX_ALLOWED_FREQUENCY;
    hal->should_set_pwm_signal_frequency_succeed = true;
    hal->should_get_pwm_signal_frequency_succeed = true;
#endif

    hal->enable_pin_state      = false;
    hal->current_sense_voltage = 0;
    hal->pwm_signal_pecentage  = 0;
#ifdef BTS7960_ENABLE_FREQUENCY_CONTROL
    hal->pwm_signal_frequency = BTS7960_HAL_MOCK_DEFAULT_MIN_ALLOWED_FREQUENCY;
#endif
  }

  return hal->should_init_succeed;
}

bool BTS7960_HAL_deInitializeHardware(BTS7960_HAL *const hal) {
  if (hal->should_deinit_succeed) {
    hal->should_init_succeed                          = true;
    hal->should_set_enable_pin_state_succeed          = false;
    hal->should_get_enable_pin_state_succeed          = false;
    hal->should_measure_current_sense_voltage_succeed = false;
    hal->should_set_pwm_signal_percentage_succeed     = false;
    hal->should_get_pwm_signal_percentage_succeed     = false;
#ifdef BTS7960_ENABLE_FREQUENCY_CONTROL
    hal->min_allowed_frequency                   = 0;
    hal->max_allowed_frequency                   = 0;
    hal->should_set_pwm_signal_frequency_succeed = false;
    hal->should_get_pwm_signal_frequency_succeed = false;
#endif

    hal->enable_pin_state      = false;
    hal->current_sense_voltage = 0;
    hal->pwm_signal_pecentage  = 0;
#ifdef BTS7960_ENABLE_FREQUENCY_CONTROL
    hal->pwm_signal_frequency = 0;
#endif
  }

  return hal->should_deinit_succeed;
}

bool BTS7960_HAL_setEnablePinState(BTS7960_HAL *const hal, bool const state) {
  if (hal->should_set_enable_pin_state_succeed) {
    hal->enable_pin_state = state;
  }

  return hal->should_set_enable_pin_state_succeed;
}

bool BTS7960_HAL_getEnablePinState(BTS7960_HAL *const hal, bool *const state) {
  if (hal->should_get_enable_pin_state_succeed) {
    *state = hal->enable_pin_state;
  }

  return hal->should_get_enable_pin_state_succeed;
}

bool BTS7960_HAL_measureCurrentSenseVoltage(BTS7960_HAL *const hal, uint32_t *const voltage) {
  if (hal->should_measure_current_sense_voltage_succeed) {
    *voltage = hal->current_sense_voltage;
  }

  return hal->should_measure_current_sense_voltage_succeed;
}

bool BTS7960_HAL_setPwmSignalPercentage(BTS7960_HAL *const hal, uint8_t const percentage) {
  if (hal->should_set_pwm_signal_percentage_succeed) {
    hal->pwm_signal_pecentage = percentage;
  }

  return hal->should_set_pwm_signal_percentage_succeed;
}

bool BTS7960_HAL_getPwmSignalPercentage(BTS7960_HAL *const hal, uint8_t *const percentage) {
  if (hal->should_get_pwm_signal_percentage_succeed) {
    *percentage = hal->pwm_signal_pecentage;
  }

  return hal->should_get_pwm_signal_percentage_succeed;
}

#ifdef BTS7960_ENABLE_FREQUENCY_CONTROL
BTS7960_HAL_FrequencyStatus BTS7960_HAL_setPwmSignalFrequency(BTS7960_HAL *const hal, uint32_t const frequency) {
  if (hal->should_set_pwm_signal_frequency_succeed) {
    hal->pwm_signal_frequency = frequency;
  }

  return hal->should_set_pwm_signal_frequency_succeed;
}

bool BTS7960_HAL_getPwmSignalFrequency(BTS7960_HAL *const hal, uint32_t *const frequency) {
  if (hal->should_get_pwm_signal_frequency_succeed) {
    *frequency = hal->pwm_signal_frequency;
  }

  return hal->should_get_pwm_signal_frequency_succeed;
}
#endif
