#include "bts7960.h"

BTS7960_Status BTS7960_initialize(BTS7960 *const bts, BTS7960_HAL *const hal) { return BTS7960_OK; }

BTS7960_Status BTS7960_deInitialize(BTS7960 *const bts) { return BTS7960_OK; }

BTS7960_Status BTS7960_enable(BTS7960 *const bts) { return BTS7960_OK; }

BTS7960_Status BTS7960_disable(BTS7960 *const bts) { return BTS7960_OK; }

bool BTS7960_isEnabled(BTS7960 const *const bts) { return false; }

BTS7960_Status BTS7960_setPowerPercentage(BTS7960 *const bts, uint8_t const percent) {
  return BTS7960_OK;
}

uint8_t BTS7960_getPowerPercentage(BTS7960 const *const bts) { return 0; }

#ifdef BTS7960_ENABLE_FREQUENCY_CONTROL
BTS7960_Status BTS7960_setOutputFrequency(BTS7960 *const bts, uint32_t const frequency) {
  return BTS7960_OK;
}

uint32_t BTS7960_getOutputFrequency(BTS7960 const *const bts) { return 0; }
#endif

BTS7960_Status BTS7960_getStatus(BTS7960 const *const bts) { return BTS7960_OK; }
