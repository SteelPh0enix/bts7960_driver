/// @file mock.h
/// BTS7960 HAL mock for testing and playing with.
/// This mock functions as an fully-configurable software simulation of BTS7960 HAL.
#pragma once

#include "../bts7960_hal.h"

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef BTS7960_ENABLE_FREQUENCY_CONTROL
  static uint32_t const BTS7960_HAL_MOCK_DEFAULT_MIN_ALLOWED_FREQUENCY = 1000;
  static uint32_t const BTS7960_HAL_MOCK_DEFAULT_MAX_ALLOWED_FREQUENCY = 100000;
#endif

  struct BTS7960_HAL_impl {
    bool should_init_succeed;
    bool should_deinit_succeed;
    bool should_set_enable_pin_state_succeed;
    bool should_get_enable_pin_state_succeed;
    bool should_measure_current_sense_voltage_succeed;
    bool should_set_pwm_signal_percentage_succeed;
    bool should_get_pwm_signal_percentage_succeed;
#ifdef BTS7960_ENABLE_FREQUENCY_CONTROL
    uint32_t min_allowed_frequency;
    uint32_t max_allowed_frequency;
    bool     should_set_pwm_signal_frequency_succeed;
    bool     should_get_pwm_signal_frequency_succeed;
#endif
    bool     enable_pin_state;
    uint32_t current_sense_voltage;
    uint8_t  pwm_signal_pecentage;
#ifdef BTS7960_ENABLE_FREQUENCY_CONTROL
    uint32_t pwm_signal_frequency;
#endif
  };

#ifdef __cplusplus
}
#endif
