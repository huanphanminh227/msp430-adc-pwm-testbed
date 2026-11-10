# TI MSP430 Bare-Metal ADC & PWM Testbed

An embedded C firmware project developed in **Code Composer Studio (CCS)** for the **Texas Instruments MSP430G2553** microcontroller. The system digitizes analog sensor voltage via an onboard 10-bit ADC and dynamically maps the data to a high-precision PWM output on Timer_A, verified on a digital storage oscilloscope.

---

## Technical Highlights
* **Bare-Metal Register Configuration:** Directly programmed MSP430 peripheral registers (`ADC10CTL0`, `ADC10CTL1`, `TACTL`, `CCTL1`) without bloated abstraction layers.
* **10-bit ADC Sampling:** Configured channel A0 with internal sample-and-hold circuitry for low-noise sensor signal acquisition.
* **Hardware Timer_A PWM:** Configured in Up-Mode using `OUTMOD_7` (Reset/Set mode) to produce a jitter-free 1 kHz carrier wave.
* **Oscilloscope Waveform Verification:** Validated linear modulation across **10% to 90% duty cycles** under varying analog input voltages.

---

## Hardware Pinout
| Function | MSP430 Pin | Hardware Description |
| :--- | :--- | :--- |
| **ADC Analog In** | `P1.0 / A0` | Potentiometer / Analog Sensor Rail (0V - 3.3V) |
| **PWM Waveform Out** | `P1.2 / TA0.1`| Oscilloscope Probe (Channel 1) / Actuator Driver |
| **GND** | `GND` | Ground Reference |

---

## Oscilloscope Setup & Measurements
1. Connect oscilloscope CH1 probe to `P1.2` and ground clip to MSP430 `GND`.
2. Configure vertical scale to **1.0 V/div** and timebase to **200 us/div** (to capture 2 complete 1 kHz cycles).
3. Set trigger to **Rising Edge** at **1.65 V**.
4. Observed duty cycle transitions smoothly from $10\%$ ($T_{ON} \approx 100\,\mu\text{s}$) to $90\%$ ($T_{ON} \approx 900\,\mu\text{s}$) when varying the input potentiometer.

---

## Building & Flashing
1. Open **Code Composer Studio (CCS)**.
2. Select **File -> Import -> CCS Projects** and navigate to this directory.
3. Select the compiler toolchain for `MSP430G2553`.
4. Connect the MSP430 LaunchPad via USB and click **Debug** (`F11`) to flash the firmware.

---
## License
Distributed under the MIT License.
