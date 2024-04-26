/// @file mock.c
/// BTS7960 HAL mock for testing and playing with.
/// This mock functions as an fully-configurable software simulation of typical MCU hardware
/// required for BTS7960 to operate.

#include "bts7960_hal.h"

struct BTS7960_HAL_impl {};

bool BTS7960_HAL_configureHardware(BTS7960_HAL *const hal) { return false; }

bool BTS7960_HAL_disableHardware(BTS7960_HAL *const hal) { return false; }

bool BTS7960_HAL_setPwmSignalPercentage(BTS7960_HAL *const hal, uint8_t const percentage) {
  return false;
}

bool BTS7960_HAL_setPwmSignalFrequency(BTS7960_HAL *const hal, uint32_t const frequency) {
  return false;
}

uint16_t BTS7960_HAL_measureCurrentSenseVoltage(BTS7960_HAL *const hal) { return 0; }
