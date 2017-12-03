# **ECSE 426 - Microprocessor Systems Fall 2017**

## Project

Develop a system to explore the interaction of embedded peripherals and sensors with cloud-enabled services.

The F4-Discovery board will be augmented by a Bluetooth Low Energy (BLE) connection to connect to Internet via a smartphone. The BLE interface will be realized through a STM32F401RE Nucleo board, along with an IDB04A1 BLE daughter board, which will be connected to the Discovery board by a serial link. The BLE module should transmit and receive data between the F4 Discovery and the smartphone. The smartphone will employ the cloud services for upload, download and processing of the files.  

 

The system will allow the board to send audio data and other optional data such as a push button status, over the BLE connection to the smartphone device. This data will be saved in a file and uploaded to the cloud services.

## Functionality

1. _STM Discovery board_
	- Read the audio data
	- Apply processing if needed
	- Transmit data serially to the Nucleo board (UART protocol)

2. _STM Nucleo board with BLE board_
	- Interconnects Discovery board with smartphone. Provides BLE functionality to the Discovery board

3. _Smartphone_
	- Interconnects Nucleo board with cloud service. Accesses cloud services, including the authentication. Sends processed data back to Nucleo board if needed

4. _Cloud Service_ 
	- Stores Discovery board data, manipulated files, makes data visible to clients on any platform, and apply basic processing such as filtering
	- Bonus: Performs advanced processing such as voice recognition and decision

## Components
- STM43F4 bicsovery board
- STM32F401RE Nucleo board
- X-NUCLEO-IDB04A1 BLE daughter board
- Sparkfun Electret Microphone Breakout

## Tools

   ### Development
   - Keil uVision
   - Android Studio
   - Google Firebase Cloud Service

   ### Project Management and Communication
   - Git
   - Slack
   - Google Drive
