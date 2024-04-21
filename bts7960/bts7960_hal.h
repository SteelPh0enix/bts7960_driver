#pragma once

/// @file bts7960_hal.h
/// BTS7690 hardware abstraction layer
/// You have to provide an implementation of BTS_7960_HAL structure, and all the related functions
/// yourself. You can either use a pre-existing implementation, or provide your own.
/// Implementation can be selected via a macro.
/// It's recommended to store informations about errors in HAL structure for error handling.
/// HAL implementation may also provide additional functions, for example PWM frequency control,
/// if available on target platfom.
/// @important **HAL structure must be initialized manually by the user before using BTS7960
///            driver!**
/// @note When using high-frequency PWM as input: BTS7960 input signal frequency limit depends on
///       the selected slew resistor value, see the datasheet section `4.2.2 Switching Times` for
///       more details.

/// Declaration of user-defined BTS7960 HAL structure.
typedef struct BTS7960_HAL_t BTS7960_HAL;

/// Initializes the hardware required for BTS7960 to operate.
/// @param[in] hal Initialized BTS7960 HAL instance.
/// @retval true The hardware was configured successfully.
/// @retval false There was an error during hardware configuration.
bool BTS7960_HAL_ConfigureHardware(BTS7960_HAL *const hal);

/// Deinitialies the hardware required for BTS7960 to operate.
/// If deinitialization is not required, it may be left empty.
/// @param[in] hal Initialized BTS7960 HAL instance.
/// @retval true The hardware was deinitialized successfully.
/// @retval false There was an error during hardware deinitialization.
bool BTS7960_HAL_DisableHardware(BTS7960_HAL *const hal);


