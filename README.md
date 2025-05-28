 # Firmware Over-The-Air (FOTA) System with STM32, Firebase, and ESP32
![System Diagram](fota.png)
This project implements a secure FOTA (Firmware Over-The-Air) update system for STM32 microcontrollers using an ESP32 module. Firmware is stored on Firebase, encrypted using AES, and transferred to an external STM32 ECU over UART.

## Over-the-air updates
After deploying an IoT device, it doesn’t mean the development is finished forever. On the contrary,
this is the most important stage in the life cycle of an IoT product and still requires active
development. We might want to add new features as a response to the users’ needs or it might be
a necessity to improve the product security after discovering a potential risk. In either case, we
need to have a means to update the firmware remotely without physically touching the deployed
IoT products. Over-the-air (OTA) update techniques provide this capability.

## Benefits of FOTA updates
Firmware over-the-air updates provide multiple benefits both to users and device vendors:
1. User convenience: Users do not have to manually check for updates and do not have to connect their devices to their computers and download special software to update them.
2. Improved security: Devices that are up to date are more secure, and updates will not be “put off” until a later time, or forgotten about.
3. New features, bug fixes, and better device performance: FOTA ensures that users have the latest bug fixes and features, improving functionality, stability, and performance.
4. Better outcomes for manufacturers: Rather than recalling faulty devices, manufacturers can deploy firmware over-the-air updates to fix devices in the field. They can also test updates on a subset of users who have opted in, to make sure that updates are reliable before wider deployment, improving reliability and brand perception.

## FOTA Implementation
### Connection between Server and ECU
I used ESP32 as telematic unit for connection between server(Firebase) and ECU(STM32F103 Blue Pill)
### Graphical User Interface
![System Diagram](images/gui.png)
KivyMD-based GUI allows uploading firmware to Firebase to make it easier for the user to upload updates to firebase and the relate information of the update.

