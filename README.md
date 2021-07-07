# JHU-PSI-Extension

## Tools/libraries used and recommended

Visual Studio Code + PlatformIO, Visual Studio, Conda

## This repo contains examples demonstrating the following functionality:

### Sound data collection (With Arduino Nano Ble Sense 33)

C++ file: collects sound data via the PDM sensor. Also has code which reads a start and a stop signal from the serial port then sends all data collected in that time period over the serial port.

Python file: sends a start and stop signal over the serial port then reads data sent over the serial port. Then sends this data to Microsoft Psi

C# file: reads data sent by python file into Microsoft Psi
