# MSP432E401Y Embedded Development

## Overview

This repository contains source code for various projects I have flashed onto the MSP432E401Y microcontroller. The projects in this repository are developed and run using the Keil uVision Integrated Development Environment.
* Datasheet: [MSP432E401Y SimpleLink™ Ethernet Microcontroller datasheet
](https://www.ti.com/lit/ds/symlink/msp432e401y.pdf?ts=1680591182321&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FMSP432E401Y) 
* Technical Reference Manual: [MSP432E4 SimpleLink™ Microcontrollers Technical Reference Manual (Rev. A)
](https://www.ti.com/lit/ug/slau723a/slau723a.pdf?ts=1680543144407&ref_url=https%253A%252F%252Fwww.google.com%252F)

## Summary of Contents

### [L1-Digital-Signals](L1-Digital-Signals)
* The objective of this project was to demonstrate a better understanding of embedded hardware architecture and programming it to detect and manipulate simple digital signals using assembly language. The completed project showcases the ability to effectively program the hardware to perform these tasks.

### [L2-Finite-State-Machine-and-Digital-Design](L2-Finite-State-Machine-and-Digital-Design)
* The objective of this project was to showcase the incorporation of digital design methodology for the development of embedded systems using a Finite-State-Machine (FSM) approach. Through the extension of knowledge of GPIO and assembly language programming, the completed project demonstrates a better understanding of the embedded hardware architecture and the ability to program it to effectively detect and manipulate digital signals.

### [L3-Analog-Signals](L3-Analog-Signals)
* The objective of this project was to demonstrate a better understanding of the embedded hardware architecture and the ability to program it to acquire analog signals using C language cross-assembled for the ARM. Through the completion of the project, proficiency in programming the hardware to acquire analog signals using C language has been showcased.

### [L4-Duty-Cycle-and-Pulse-Timing](L4-Duty-Cycle-and-Pulse-Timing)
* The objective of this project was to showcase the use of embedded properties and timing through the utilization of C language cross-assembled for ARM. The completed project effectively demonstrates timing by incorporating LEDs and a stepping motor.

### [L5-Peripheral-Interfacing](L5-Peripheral-Interfacing)
* The objective of this project was to showcase the ability to interface peripherals and integrate embedded concepts. Specifically, the completed project demonstrates proficiency in decoding keypad input through a functional user interface that has been created via hardware and software interfacing.

### [L6-Embedded-Integration](L6-Embedded-Integration)
* The objective of this project was to effectively integrate embedded components in order to create a functional system. Through the combination of previous labs and studios, the core project platform was created, which includes stepping motor, push button inputs, and LED outputs. 

### [L7-Interrupts-and-Event-Based-Programming](L7-Interrupts-and-Event-Based-Programming)
* The objective of this project was to gain experience with event-based programming. Through the use of GPIO and button interrupts, the completed project demonstrates proficiency in implementing event-based programming. Additionally, the project showcases the ability to output data through UART.

### [L8-Collecting-Distance-Data](L8-Collecting-Distance-Data)
* The aim of this project was to communicate with a digital sensor (TOF VL53L1X) and collect data from it using I2C communication. The completed project demonstrates the ability to effectively communicate with the sensor and collect and process data. Additionally, the project utilizes Pyserial and Open3D to visualize the collected data.

## Notes

If a Keil project fails to locate path to the respective source file to flash to the microcontroller, the path can be updated by right-clicking the respective .c file and updating its path within the project.

## Acknowledgements

* Text:“Embedded Systems: Introduction to ARM Cortex-M Microcontrollers", 5th edition,
6th printing, January 2019, by Valvano 

* S. Athar, T. E. Doyle, and Y. Haddara, Computer Engineering 2DX3 2022-2023 Laboratory Manual. Hamilton: 
DEPARTMENT OF ELECTRICAL AND COMPUTER ENGINEERING, MCMASTER
UNIVERSITY, 2023.
