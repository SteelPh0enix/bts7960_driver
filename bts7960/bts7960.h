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
/// All of the functions will return a BTS7960_Result value. Whether it's reasonable to check it
/// depends on HAL implementation. For example, if setting the I/O pin state can never fail, the
/// function will also never fail (except for very obvious reasons), so there's no reason to check
/// the returned status if you're sure it won't fail due to software issue. However, it's
/// recommended to validate the return value of those functions whenever possible anyway, as
/// compiler optimizations should detect and remove branches of code that can never be executed in
/// final binary, making it a free "safety net" for the future.
///
/// In **extreme** scenario, one may put some of those "obvious" and verbose checks behind a macro.
/// But for now, this is good enough.

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>

#include "bts7960_hal.h"

/// BTS7960 instance.
typedef struct BTS7960_t {
  BTS7960_HAL *hal;              ///< Pointer to a HAL instance.
  uint16_t fault_voltage;        ///< Voltage on current sense pin when driver is in fault mode,
                                 ///< in millivolts.
  uint16_t current_sense_ratio;  ///< Current sense ratio.
  bool is_initialized;           ///< Flag set by `Initialize` to indicate readiness.
} BTS7960;

/// BTS7960 state, returned by BTS7960_checkState() function.
typedef struct BTS7960_State_t {
  uint16_t current;  ///< Current flowing through the driver, in milliamperes.
  bool fault;        ///< If true, the driver is currently in fault mode.
} BTS7960_Status;

/// Enumeration representing a result of performed operation.
typedef enum BTS7960_Result_t {
  BTS7960_OK,                         ///< Operation was successful.
  BTS7960_HAL_ERROR,                  ///< HAL error, details should be saved in HAL instance.
  BTS7960_ALREADY_INITIALIZED,        ///< HAL tried to be re-initialized.
  BTS7960_NOT_INITIALIZED,            ///< HAL tried to be used before initialization.
  BTS7960_ERROR_INVALID_POWER_VALUE,  ///< Invalid value was passed to power-related function.
#ifdef BTS7960_ENABLE_FREQUENCY_CONTROL
  BTS7960_ERROR_FREQUENCY_TOO_LOW,   ///< The frequency is too low for the hardware to generate.
  BTS7960_ERROR_FREQUENCY_TOO_HIGH,  ///< The frequency is too high for the hardware to generate.
#endif
  BTS7960_FAULT_DETECTED,  ///< A fault was detected during status check.
} BTS7960_Result;


/// Initializes the driver structure and underlying hardware.
/// @important You should call this function first.
/// @important Provided HAL instance should be initialized manually by the user
///            before passing it to this function.
/// @param[in] bts Pointer to BTS7960 driver instance.
/// @param[in] hal Pointer to user-provided HAL instance.
/// @param[in] fault_voltage Voltage on current-sense pin in fault mode, in millivolts. The current
///                          sense resistor value should guarantee that this voltage will not rise
///                          above maximum voltage tolerated by ADC. You've been warned.
/// @param[in] current_sense_ratio Current sense ratio, default is stored in
///                                BTS7960_DEFAULT_CURRENT_SENSE_RATIO constant.
/// @retval BTS7960_OK If initialization was successful.
/// @retval BTS7960_ALREADY_INITIALIZED If the instance is already initialized.
/// @retval BTS7960_HAL_ERROR If initialization failed due to an internal HAL error.
BTS7960_Result BTS7960_initialize(BTS7960 *const bts, BTS7960_HAL *const hal,
                                  const uint16_t fault_voltage, const uint16_t current_sense_ratio);

/// De-initialize the driver, freeing the underlying hardware.
/// @param[in] bts Pointer to BTS7960 driver instance.
/// @retval BTS7960_OK If de-initialization was successful.
/// @retval BTS7690_NOT_INITIALIZED If driver is not initialized.
/// @retval BTS7960_HAL_ERROR If de-initialization failed due to an internal HAL error.
BTS7960_Result BTS7960_deInitialize(BTS7960 *const bts);

/// Enables the driver's output.
/// @param[in] bts Pointer to BTS7960 driver instance.
/// @retval BTS7960_OK If enabling succeeded.
/// @retval BTS7690_NOT_INITIALIZED If driver is not initialized.
/// @retval BTS7960_HAL_ERROR If enabling failed due to an internal HAL error.
BTS7960_Result BTS7960_enable(BTS7960 *const bts);

/// Disables the driver's output.
/// @param[in] bts Pointer to BTS7960 driver instance.
/// @retval BTS7960_OK If disabling succeeded.
/// @retval BTS7690_NOT_INITIALIZED If driver is not initialized.
/// @retval BTS7960_HAL_ERROR If disabling failed due to an internal HAL error.
BTS7960_Result BTS7960_disable(BTS7960 *const bts);

/// Checks if the driver is currently enabled.
/// Output parameters are cleared/set to 0 on error.
/// @param[in] bts Pointer to BTS7960 driver instance.
/// @param[out] status `true` if driver is enabled, `false` otherwise.
/// @retval BTS7960_OK If getting the status succeeded.
/// @retval BTS7690_NOT_INITIALIZED If driver is not initialized.
/// @retval BTS7960_HAL_ERROR If getting the status failed due to an internal HAL error.
BTS7960_Result BTS7960_isEnabled(BTS7960 const *const bts, bool *const status);

/// Measures the current flowing through BTS7960 driver and returns it (in milliamps).
/// Also informs if a fault happened.
/// Output parameters are set to 0 on error, except in BTS7960_FAULT_DETECTED case, where the
/// current is set to 0 but the flag remains true.
/// @param[in] bts Pointer to BTS7960 driver instance.
/// @param[out] status Current status of the driver.
/// @retval BTS7960_OK If getting the status succeeded.
/// @retval BTS7960_FAULT_DETECTED If a driver's fault is detected.
/// @retval BTS7690_NOT_INITIALIZED If driver is not initialized.
/// @retval BTS7960_HAL_ERROR If getting the status failed due to an internal HAL error.
BTS7960_Result BTS7960_getStatus(BTS7960 const *const bts, BTS7960_Status *const status);

/// Sets the power output of driver in 0-100% range.
/// @param[in] bts Pointer to BTS7960 driver instance.
/// @param[in] percent Percentage of driver's power in 0-100 range.
/// @retval BTS7960_OK If setting the power succeeded.
/// @retval BTS7690_NOT_INITIALIZED If driver is not initialized.
/// @retval BTS7960_ERROR_INVALID_POWER_VALUE If `percent` is outside of [0, 100] range.
/// @retval BTS7960_HAL_ERROR If setting the power failed due to an internal HAL error.
BTS7960_Result BTS7960_setPowerPercentage(BTS7960 *const bts, uint8_t const percent);

/// Gets current power output of BTS7960 in 0-100[%] range (with integer precision).
/// Output parameters are cleared/set to 0 on error.
/// @param[in] bts Pointer to BTS7960 driver instance.
/// @param[out] percentage Current power output in 0-100[%] range.
/// @retval BTS7960_OK If getting the power succeeded.
/// @retval BTS7690_NOT_INITIALIZED If driver is not initialized.
/// @retval BTS7960_HAL_ERROR If getting the power failed due to an internal HAL error.
BTS7960_Result BTS7960_getPowerPercentage(BTS7960 const *const bts, uint8_t *const percentage);

#ifdef BTS7960_ENABLE_FREQUENCY_CONTROL
/// Sets the output signal frequency.
/// @param[in] bts Pointer to BTS7960 driver instance.
/// @param[in] frequency Frequency of the signal, in hertz.
/// @retval BTS7960_OK If setting the frequency succeeded.
/// @retval BTS7690_NOT_INITIALIZED If driver is not initialized.
/// @retval BTS7960_ERROR_FREQUENCY_TOO_LOW If `frequency` is too low.
/// @retval BTS7960_ERROR_FREQUENCY_TOO_HIGH If `frequency` is too high.
/// @retval BTS7960_HAL_ERROR If setting the frequency failed due to an internal HAL error.
BTS7960_Result BTS7960_setOutputFrequency(BTS7960 *const bts, uint32_t const frequency);

/// Gets the current output signal frequency.
/// Output parameters are cleared/set to 0 on error.
/// @param[in] bts Pointer to BTS7960 driver instance.
/// @param[out] frequency Frequency of the signal, in hertz.
/// @retval BTS7960_OK If getting the frequency succeeded.
/// @retval BTS7690_NOT_INITIALIZED If driver is not initialized.
/// @retval BTS7960_HAL_ERROR If getting the frequency failed due to an internal HAL error.
BTS7960_Result BTS7960_getOutputFrequency(BTS7960 const *const bts, uint32_t *const frequency);
#endif
