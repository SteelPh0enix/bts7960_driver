#include "bts7960.h"

#include <stddef.h>

#ifndef BTS7960_DISABLE_ASSERTS
#include <assert.h>
#endif

BTS7960_Result BTS7960_initialize(BTS7960 *const bts, BTS7960_HAL *const hal,
                                  const uint16_t fault_voltage,
                                  const uint16_t current_sense_ratio) {
#ifndef BTS7960_DISABLE_ASSERTS
  assert(bts);
  assert(hal);
  assert(fault_voltage);
  assert(current_sense_ratio);
#endif

  if (bts->is_initialized) {
    return BTS7960_ALREADY_INITIALIZED;
  }

  if (!BTS7960_HAL_initializeHardware(hal)) {
    return BTS7960_HAL_ERROR;
  }

  bts->hal = hal;
  bts->fault_voltage = fault_voltage;
  bts->current_sense_ratio = current_sense_ratio;
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

  bts->hal = NULL;
  bts->fault_voltage = 0;
  bts->current_sense_ratio = 0;
  bts->is_initialized = false;

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

  uint16_t voltage = 0;

  if (!BTS7960_HAL_measureCurrentSenseVoltage(bts->hal, &voltage)) {
    return BTS7960_HAL_ERROR;
  }

  // Now that we have the voltage, we need to check what it means.

  return BTS7960_OK;
}
