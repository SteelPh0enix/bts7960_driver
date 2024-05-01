#include "bts7960.h"

#include <stddef.h>

#ifndef BTS7960_DISABLE_ASSERTS
#include <assert.h>
#endif

BTS7960_Result BTS7960_initialize(BTS7960 *const bts, BTS7960_HAL *const hal) {
  return BTS7960_advancedInitialize(
      bts, hal, BTS7960_DEFAULT_CURRENT_SENSE_RESISTANCE, BTS7960_DEFAULT_CURRENT_SENSE_RATIO,
      BTS7960_DEFAULT_CURRENT_IN_FAULT_MODE, BTS7960_DEFAULT_FAULT_VOLTAGE_TOLERANCE);
}

BTS7960_Result BTS7960_advancedInitialize(BTS7960 *const bts, BTS7960_HAL *const hal,
                                          const uint32_t current_sense_resistance,
                                          const uint16_t current_sense_ratio,
                                          const uint16_t current_in_fault_mode,
                                          const uint8_t fault_voltage_tolerance) {
#ifndef BTS7960_DISABLE_ASSERTS
  assert(bts);
  assert(hal);
  assert(current_sense_resistance);
  assert(current_sense_ratio);
  assert(current_in_fault_mode);
#endif

  if (bts->is_initialized) {
    return BTS7960_ALREADY_INITIALIZED;
  }

  if (!BTS7960_HAL_initializeHardware(hal)) {
    return BTS7960_HAL_ERROR;
  }

  bts->hal = hal;
  bts->current_sense_resistance = current_sense_resistance;
  // U = I * R, current in microamps -> /10^3 to convert result to millivolts
  bts->fault_voltage = current_sense_resistance * current_in_fault_mode / 1000;
  bts->current_sense_ratio = current_sense_ratio;
  bts->current_in_fault_mode = current_in_fault_mode;
  bts->fault_voltage_tolerance = fault_voltage_tolerance;
  bts->is_initialized = true;

  return BTS7960_OK;
}

BTS7960_Result BTS7960_deInitialize(BTS7960 *const bts) {
#ifndef BTS7960_DISABLE_ASSERTS
  assert(bts);
#endif

  if (!bts->is_initialized) {
    return BTS7960_NOT_INITIALIZED;
  }

  if (!BTS7960_HAL_deInitializeHardware(bts->hal)) {
    return BTS7960_HAL_ERROR;
  }

  bts->is_initialized = false;
  bts->hal = NULL;
  bts->current_sense_resistance = 0;
  bts->fault_voltage = 0;
  bts->current_sense_ratio = 0;
  bts->current_in_fault_mode = 0;
  bts->fault_voltage_tolerance = 0;

  return BTS7960_OK;
}

BTS7960_Result BTS7960_enable(BTS7960 *const bts) {
#ifndef BTS7960_DISABLE_ASSERTS
  assert(bts);
#endif

  if (!bts->is_initialized) {
    return BTS7960_NOT_INITIALIZED;
  }

  if (!BTS7960_HAL_setEnablePinState(bts->hal, true)) {
    return BTS7960_HAL_ERROR;
  }

  return BTS7960_OK;
}

BTS7960_Result BTS7960_disable(BTS7960 *const bts) {
#ifndef BTS7960_DISABLE_ASSERTS
  assert(bts);
#endif

  if (!bts->is_initialized) {
    return BTS7960_NOT_INITIALIZED;
  }

  if (!BTS7960_HAL_setEnablePinState(bts->hal, false)) {
    return BTS7960_HAL_ERROR;
  }

  return BTS7960_OK;
}

BTS7960_Result BTS7960_isEnabled(BTS7960 const *const bts, bool *const status) {
#ifndef BTS7960_DISABLE_ASSERTS
  assert(bts);
  assert(status);
#endif

  *status = false;

  if (!bts->is_initialized) {
    return BTS7960_NOT_INITIALIZED;
  }

  if (!BTS7960_HAL_getEnablePinState(bts->hal, status)) {
    return BTS7960_HAL_ERROR;
  }

  return BTS7960_OK;
}

BTS7960_Result BTS7960_getStatus(BTS7960 const *const bts, BTS7960_Status *const status) {
#ifndef BTS7960_DISABLE_ASSERTS
  assert(bts);
  assert(status);
#endif

  status->fault = false;
  status->current = 0;

  if (!bts->is_initialized) {
    return BTS7960_NOT_INITIALIZED;
  }

  uint32_t voltage = 0;

  if (!BTS7960_HAL_measureCurrentSenseVoltage(bts->hal, &voltage)) {
    return BTS7960_HAL_ERROR;
  }

  // Now that we have the voltage, we need to check what it means.
  // The external resistor Ris determines the voltage per output current.
  // With the nominal value of current-sense ratio `k=8500`, a `Ris=1000Ω` leads to:
  // `Vis = (Il / 8.5A)V`.
  // If we substitute `k` and `Ris` into this formula, we get:
  // `Vis = (Il / (k * R / 10^6)A)V`
  // We can calculate the load current `Il` by transforming this formula:
  // `Il = (k * R / 10^6) * Vis`
  // This will give us the current in amperes. I'd prefer a milliampere precision to prevent using
  // floating-point numbers, so instead i'll use 10^3 instead of 10^6 in denominator.
  // But first, we need to check if the fault has happened.


  return BTS7960_OK;
}
