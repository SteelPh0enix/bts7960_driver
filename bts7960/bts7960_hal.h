#pragma once

/// @file bts7960_hal.h
/// BTS7690 hardware abstraction layer
/// You have to provide an implementation of BTS_7960_HAL structure, and all the related functions
/// yourself. You can either use a pre-existing implementation, or provide your own.
/// Implementation must be linked manually.
/// It's recommended to store informations about errors in HAL structure for error handling or
/// debugging. HAL implementation may also provide additional functions, for example PWM frequency
/// control, if available on target platform.
/// @important **HAL instance must be initialized manually by the user before using BTS7960
///            driver!**
/// @note When using high-frequency PWM as input: BTS7960 input signal frequency limit depends on
///       the selected slew resistor value, see the datasheet section `4.2.2 Switching Times` for
///       more details.

#include <stdbool.h>
#include <stdint.h>

/// Declaration of user-defined BTS7960 HAL structure.
typedef struct BTS7960_HAL_impl BTS7960_HAL;

#ifdef BTS7960_ENABLE_FREQUENCY_CONTROL
typedef enum {
  BTS7960_HAL_FREQUENCY_OK,
  BTS7960_HAL_FREQUENCY_TOO_LOW,
  BTS7960_HAL_FREQUENCY_TOO_HIGH,
} BTS7960_HAL_FrequencyStatus;
#endif

/// Initializes the hardware required for BTS7960 to operate.
/// @param[in] hal Initialized BTS7960 HAL instance.
/// @retval true The hardware was configured successfully.
/// @retval false There was an error during hardware configuration.
bool BTS7960_HAL_initializeHardware(BTS7960_HAL *const hal);

/// De-initializes the hardware required for BTS7960 to operate.
/// @param[in] hal Initialized BTS7960 HAL instance.
/// @retval true The hardware was de-initialized successfully.
/// @retval false There was an error during hardware de-initialization.
bool BTS7960_HAL_deInitializeHardware(BTS7960_HAL *const hal);

/// Sets driver's `enable` pin state.
/// @param[in] hal Initialized BTS7960 HAL instance.
/// @param[in] state State of the pin.
/// @retval true The pin's state has been set.
/// @retval false The pin's state couldn't be set.
bool BTS7960_HAL_setEnablePinState(BTS7960_HAL *const hal, bool const state);

/// Gets driver's `enable` pin state.
/// @param[in] hal Initialized BTS7960 HAL instance.
/// @param[out] state Current state of `enable` pin.
/// @retval true State of the pin has been fetched.
/// @retval false Couldn't fetch the state of the pin.
bool BTS7960_HAL_getEnablePinState(BTS7960_HAL *const hal, bool *const state);

/// Returns the voltage on current sense (IS) pin in millivolts.
/// @param[in] hal Initialized BTS7960 HAL instance.
/// @param[out] voltage Voltage on the current sense pin, in millivolts.
/// @retval true The voltage has been measured successfully.
/// @retval false Couldn't measure the current sense voltage.
bool BTS7960_HAL_measureCurrentSenseVoltage(BTS7960_HAL *const hal, uint32_t *const voltage);

/// Sets the PWM signal duty cycle percentage.
/// @param[in] hal Initialized BTS7960 HAL instance.
/// @param[in] percentage PWM signal duty cycle percentage.
/// @retval true PWM signal duty cycle has been set.
/// @retval false Couldn't set the PWM signal duty cycle.
bool BTS7960_HAL_setPwmSignalPercentage(BTS7960_HAL *const hal, uint8_t const percentage);

/// Gets the PWM signal duty cycle percentage.
/// @param[in] hal Initialized BTS7960 HAL instance.
/// @param[out] percentage PWM signal duty cycle percentage.
/// @retval true PWM signal duty cycle has been fetched.
/// @retval false Couldn't fetch the PWM signal duty cycle.
bool BTS7960_HAL_getPwmSignalPercentage(BTS7960_HAL *const hal, uint8_t *const percentage);

#ifdef BTS7960_ENABLE_FREQUENCY_CONTROL
/// Sets the PWM signal frequency.
/// @param[in] hal Initialized BTS7960 HAL instance.
/// @param[in] frequency Frequency of the PWM signal.
/// @retval BTS7960_HAL_FREQUENCY_OK PWM signal frequency has been set.
/// @retval BTS7960_HAL_FREQUENCY_TOO_LOW Requested PWM signal frequency is too low.
/// @retval BTS7960_HAL_FREQUENCY_TOO_HIGH Requested PWM signal frequency is too high.
BTS7960_FrequencyStatus BTS7960_HAL_setPwmSignalFrequency(BTS7960_HAL *const hal,
                                                          uint32_t const frequency);

/// Gets the PWM signal frequency.
/// @param[in] hal Initialized BTS7960 HAL instance.
/// @param[out] frequency Frequency of the PWM signal.
/// @retval true PWM signal frequency has been fetched.
/// @retval false Couldn't fetch the PWM signal frequency.
bool BTS7960_HAL_getPwmSignalFrequency(BTS7960_HAL *const hal, uint32_t *const frequency);
#endif
