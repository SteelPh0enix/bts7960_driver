#pragma once

/// @file bts7960.h
/// Main header for BTS7960 driver.
/// This driver provides basic interface for BTS7960 high-current PN half-bridge.
///
/// Driver's responsibility is to provide an easy-to-use interface for BTS7960.
/// This driver is capable of:
///   - Initializing and de-initializing the hardware required for BTS7960 to operate;
///   - Enabling and disabling BTS7960 via hardware enable pin;
///   - Setting the PWM signal modulation parameters (frequency and duty cycle);
///   - Checking the status of BTS7960 by monitoring the status pin;
///
/// Some of the features may not be present on all platforms, depending on HAL implementation.
/// Configuration of this driver's capabilities can be performed via macros:
///   * BTS7960_DISABLE_ASSERTS - when defined, disables asserts in library's code, along with
///                               `assert.h` library.
///   * BTS7960_ENABLE_FREQUENCY_CONTROL - when defined, enables BTS7960_setOutputFrequency()
///                                        function. Define it if your HAL supports it.
///
/// @important In order to use this library, you must provide your own HAL bindings for the target
/// platform. HAL is the library's back-end, providing control over the actual hardware of the MCU.
/// See `bts7960_hal.h` file for details.
///
/// Most of the functions will return a BTS7960_Status value. Whether it's reasonable to check it
/// depends on HAL implementation. For example, if setting the I/O pin state can never fail, the
/// function will also never fail, so there's no reason to check the returned status.
/// However, it's recommended to validate the return value of those functions whenever possible
/// anyway, as compiler optimizations should detect and remove branches of code that can never be
/// executed in final binary, making it a free "safety net" for the future.

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>

#include "bts7960_hal.h"

/// BTS7960 instance.
typedef struct BTS7960_t {
  BTS7960_HAL *hal;     ///< A pointer to HAL instance.
  bool is_initialized;  ///< Flag set by `Initialize` to indicate readiness.
} BTS7960;

/// BTS7960 state, returned by BTS7960_checkState() function.
typedef struct BTS7960_State_t {
  bool fault;        ///< If true, the driver is currently in fault mode.
  uint16_t current;  ///< Current flowing through the driver, in milliamperes.
} BTS7960_State;

/// Enumeration representing a status of performed BTS7960 driver operation.
typedef enum BTS7960_Status_t {
  BTS7960_OK,                         ///< Operation was successful.
  BTS7960_HAL_ERROR,                  ///< HAL error, details should be saved in HAL instance.
  BTS7960_ERROR_INVALID_POWER_VALUE,  ///< Invalid value was passed to power-related function.
#ifdef BTS7960_ENABLE_FREQUENCY_CONTROL
  BTS7960_ERROR_FREQUENCY_TOO_LOW,   ///< The frequency is too low for the hardware to generate.
  BTS7960_ERROR_FREQUENCY_TOO_HIGH,  ///< The frequency is too high for the hardware to generate.
#endif
  BTS7960_FAULT_DETECTED,  ///< A fault was detected during current measurements.
} BTS7960_Status;

/// Initializes BTS7960 structure and underlying hardware.
/// @important You should call this function first.
/// @important Provided HAL instance should be initialized manually by the user
///            before passing it to this function.
/// @param[in] bts Pointer to BTS7960 driver instance.
/// @param[in] hal Pointer to user-provided HAL instance.
/// @retval BTS7960_OK If initialization was successful.
/// @retval BTS7960_HAL_ERROR If initialization has fails due to internal HAL error.
BTS7960_Status BTS7960_initialize(BTS7960 *const bts, BTS7960_HAL *const hal);

/// De-initialize BTS7960, freeing the underlying hardware.
/// @param[in] bts Pointer to BTS7960 driver instance.
/// @retval BTS7960_OK If de-initialization was successful.
/// @retval BTS7960_HAL_ERROR If de-initialization has fails due to internal HAL error.
BTS7960_Status BTS7960_deInitialize(BTS7960 *const bts);

/// Enables the BTS7960 driver output.
/// When enabled, you can use BTS7960_setPowerPercentage() set power percentage,
/// or BTS7960_setRawPower() to set raw PWM value.
/// @param[in] bts Pointer to BTS7960 driver instance.
/// @retval BTS7960_OK if enabling succeeded.
/// @retval BTS7960_HAL_ERROR if enabling fails due to internal HAL error.
BTS7960_Status BTS7960_enable(BTS7960 *const bts);

/// Disables the BTS7960 driver output.
/// @param[in] bts Pointer to BTS7960 driver instance.
/// @retval BTS7960_OK if disabling succeeded.
/// @retval BTS7960_HAL_ERROR if disabling fails due to internal HAL error.
BTS7960_Status BTS7960_disable(BTS7960 *const bts);

/// Checks if the driver is currently enabled.
/// @param[in] bts Pointer to BTS7960 driver instance.
/// @retval true Driver is currently enabled.
/// @retval false Driver is currently disabled.
bool BTS7960_isEnabled(BTS7960 const *const bts);

/// Sets the power output of BTS7960 in 0-100% range (with integer precision).
/// @param[in] bts Pointer to BTS7960 driver instance.
/// @param[in] percent Percentage of power in 0-100 range.
/// @retval BTS7960_OK if setting the power succeeded.
/// @retval BTS7960_ERROR_INVALID_POWER_VALUE if `percent` is outside of [0, 100] range.
/// @retval BTS7960_HAL_ERROR if setting the power fails due to internal HAL error.
BTS7960_Status BTS7960_setPowerPercentage(BTS7960 *const bts, uint8_t const percent);

/// Gets current power output of BTS7960 in 0-100% range (with integer precision)
/// @param[in] bts Pointer to BTS7960 driver instance.
/// @returns Current power output, in percentages.
uint8_t BTS7960_getPowerPercentage(BTS7960 const *const bts);

#ifdef BTS7960_ENABLE_FREQUENCY_CONTROL
/// Sets the output signal frequency.
/// @param[in] bts Pointer to BTS7960 driver instance.
/// @param[in] frequency Frequency of the signal, in hertz.
/// @retval BTS7960_OK if setting the power succeeded.
/// @retval BTS7960_ERROR_FREQUENCY_TOO_LOW if `frequency` is too low.
/// @retval BTS7960_ERROR_FREQUENCY_TOO_HIGH if `frequency` is too high.
/// @retval BTS7960_HAL_ERROR if setting the frequency fails due to internal HAL error.
BTS7960_Status BTS7960_setOutputFrequency(BTS7960 *const bts, uint32_t const frequency);

/// Gets the current output signal frequency.
/// @param[in] bts Pointer to BTS7960 driver instance.
/// @returns Output signal frequency.
uint32_t BTS7960_getOutputFrequency(BTS7960 const *const bts);
#endif

/// Measures the current flowing through BTS7960 driver and returns it (in milliamps).
/// @param[in] bts Pointer to BTS7960 driver instance.
/// @return BTS7960_Status structure with current driver status.
BTS7960_Status BTS7960_getStatus(BTS7960 const *const bts);
