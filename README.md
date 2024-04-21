# Platform-agnostic C driver for BTS7960

This is a simple platform-agnostic driver for [BTS7960 High Current PN Half-Bridge](./BTS7960.pdf)
written in C (C11 standard).
I also provide ready-to-use STM32 HAL bindings example.

It's worth noting that popular power driver modules have **two** BTS7960's on them, in an H-bridge
configuration. This library provides some simple abstraction for H-bridges like that.

For more details, see the documentation provided in code.
For list of BTS7960 features, go to the [datasheet](./BTS7960.pdf).

## Description

This library provides an easy-to-use and easy-to-port struct-based BTS7960 driver.
This driver must be provided with an implementation of few platform-dependent functions in order
for it to work correctly.
It comes with test mocks, a dummy example, and STM32 HAL implementation.
For any other platform - you have to provide implementation yourself.

## Requirements

### Hardware

For a **single** BTS7960, following hardware is required:

* 1 digital output pin with hardware PWM generation capabilities for driving the input.
* 1 digital output pin for shutdown (optional, but recommended, can be hard-wired "high", enabling
  the driver permanently)
* 1 analog input pin for current sense and fault detection (optional, but recommended)

### Software

* C compiler capable of compiling C11-compliant code for your desired platform.

