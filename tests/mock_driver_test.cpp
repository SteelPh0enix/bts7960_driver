#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <bts7960/bts7960.h>
#include <bts7960/hal/mock.h>

TEST_GROUP(BTS7960) {
  static inline BTS7960     bts;
  static inline BTS7960_HAL hal;

  void setup() {
    hal.should_init_succeed = true;
    BTS7960_HAL_initializeHardware(&hal);
    BTS7960_initialize(&bts, &hal);
  }

  void teardown() {
    BTS7960_deInitialize(&bts);
    BTS7960_HAL_deInitializeHardware(&hal);
  }
};

/// Given a correctly initialized HAL instance,
/// When BTS7960_initialize() is called,
/// Then BTS instance is initialized with default configuration.
TEST(BTS7960, initializesCorrectly) {
  // BTS should be properly initialized in setup(), so only thing to do is
  // to check the fields.
  CHECK_TRUE(bts.is_initialized);
  POINTERS_EQUAL(&hal, bts.hal);
  UNSIGNED_LONGS_EQUAL(BTS7960_DEFAULT_CURRENT_SENSE_RESISTANCE, bts.current_sense_resistance);
  UNSIGNED_LONGS_EQUAL(BTS7960_DEFAULT_CURRENT_SENSE_RATIO, bts.current_sense_ratio);
  UNSIGNED_LONGS_EQUAL(BTS7960_DEFAULT_CURRENT_IN_FAULT_MODE, bts.current_in_fault_mode);
  UNSIGNED_LONGS_EQUAL(BTS7960_DEFAULT_FAULT_VOLTAGE_TOLERANCE, bts.fault_voltage_tolerance);

  // Fault voltage is defined as a voltage drop on current sense resistor @ fault current.
  uint32_t const expected_fault_voltage = bts.current_sense_resistance * bts.current_in_fault_mode / 1000;
  UNSIGNED_LONGS_EQUAL(expected_fault_voltage, bts.fault_voltage);

  // Fault voltage epsilon is defined as accepted voltage deviation from fault voltage to be clasified as a fault.
  uint32_t const expected_fault_voltage_epsilon = bts.fault_voltage * bts.fault_voltage_tolerance / 100;
  UNSIGNED_LONGS_EQUAL(expected_fault_voltage_epsilon, bts.fault_voltage_epsilon);
  UNSIGNED_LONGS_EQUAL(bts.fault_voltage - bts.fault_voltage_epsilon, bts.fault_voltage_min);

  // Current sense multiplier is defined as current sense ratio * current sense resistance / 1000
  uint32_t const expected_current_sense_multiplier = bts.current_sense_ratio * bts.current_sense_resistance / 1000;
  UNSIGNED_LONGS_EQUAL(expected_current_sense_multiplier, bts.current_sense_multiplier);
}

int main(int ac, char **av) { return CommandLineTestRunner::RunAllTests(ac, av); }
