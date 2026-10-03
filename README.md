# ESP32 Sine Wave Generator

This project generates a sine-wave-shaped signal using an **Arduino Nano ESP32**.

The code creates a sine lookup table containing multiple sample values of a sine wave. These values are used to continuously change the PWM duty cycle of the ESP32 output pin.

The PWM signal is generated on pin **D9** at a high frequency. By changing the PWM duty cycle according to the sine table, the output represents the shape of a sine wave.

A simple RC low-pass filter can be connected to the output pin to smooth the PWM pulses and obtain an approximately sinusoidal waveform that can be observed on an oscilloscope.

The program also sends basic information to the Serial Monitor, including the output pin, sine-wave frequency, PWM frequency, and waveform generation status.

## Main Functions of the Code

- Generates a sine lookup table using the `sin()` function.
- Uses 100 samples to represent one complete sine-wave cycle.
- Generates PWM output from Arduino Nano ESP32 pin D9.
- Uses a PWM frequency of approximately 78 kHz.
- Continuously updates the PWM duty cycle to create a sine-wave pattern.
- Produces an output sine-wave frequency of approximately 20 Hz.
- Displays waveform generation information in the Serial Monitor.
- Can be connected to an oscilloscope for waveform observation.

## Current Project Stage

Currently, the project only focuses on waveform generation and testing using the Arduino Nano ESP32.

The sonar transducer is not being used at this stage. Future versions can include potentiometer-based control, adaptive frequency selection, LFM chirp generation, and sonar transmitter hardware.
